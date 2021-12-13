#pragma once
#include <algorithm>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

template<class V, class E>
class Vertex;

template<class V, class E>
class Edge {
public:
	Edge(const E& properties, Vertex<V, E>* v1, Vertex<V, E>* v2) : properties_(properties), _v1(v1), _v2(v2) {}
	Vertex<V, E>* getVertex1() {
		return _v1;
	}
	Vertex<V, E>* getVertex2() {
		return _v2;
	}
	const E* getProperties() const {
		return &properties_;
	}

private:
	const E properties_;
	Vertex<V, E>* _v1;
	Vertex<V, E>* _v2;
};

template<class V, class E>
class Vertex {
public:
	explicit Vertex(const V& properties) : properties_(properties) {}
	const V& getProperties() const {
		return properties_;
	}
	const std::vector<Edge<V, E>*>& getEdges() const {
		return edges_;
	}
	void addOrderedEdge(const E& properties, Vertex<V, E>* target) {
		auto* edge = new Edge<V, E>(properties, target, nullptr);
		edges_.push_back(edge);
	}
	void addEdge(const E& properties, Vertex<V, E>* target) {
		auto* edge = new Edge<V, E>(properties, target, this);
		edges_.push_back(edge);
		target->edges_.push_back(edge);
	}

	~Vertex() {
		for (auto* edge : edges_) {
			if (edge->getVertex1() != nullptr && edge->getVertex2() != nullptr) {
				Vertex<V, E>* other = (edge->getVertex1() == this) ? edge->getVertex2() : edge->getVertex1();
				other->edges_.erase(std::find(other->edges_.begin(), other->edges_.end(), edge));
			}
			delete edge;
		}
	}

private:
	const V properties_;
	std::vector<Edge<V, E>*> edges_;
};

template<class V>
bool search(const Vertex<V, int>* vertex, const V& param, std::vector<const Vertex<V, int>*>* visited, int* cost) {
	if (std::find(visited->begin(), visited->end(), vertex) != visited->end()) { return false; }
	visited->push_back(vertex);
	if (vertex->getProperties() == param) { return true; }
	for (const auto* edge : vertex->getEdges()) {
		const Vertex<V, int>* next =
			edge->getVertex1() == vertex || edge->getVertex1() == nullptr ? edge->getVertex2() : edge->getVertex1();
		if (search(next, param, visited, cost)) {
			*cost += edge->getProperties();
			return true;
		}
	}
	visited->pop_back();
	return false;
}

template<class V, class E, class F>
void depthPass(const Vertex<V, E>* vertex, F& visitor) {
	if (!visitor.visitVertex(vertex)) { return; }
	for (auto* edge : vertex->getEdges()) {
		if (!visitor.visitEdge(edge)) { continue; }
		const Vertex<V, E>* next =
			edge->getVertex1() == vertex || edge->getVertex1() == nullptr ? edge->getVertex2() : edge->getVertex1();
		depthPass(next, visitor);
		visitor.leaveEdge(edge);
	}
	visitor.leaveVertex(vertex);
}

template<class V, class E>
class VisitorOneTime {
public:
	virtual bool visitVertex(const Vertex<V, E>* vertex) {
		if (std::find(visited_.begin(), visited_.end(), vertex) != visited_.end()) { return false; }
		visited_.push_back(vertex);
		return true;
	}
	virtual bool visitEdge(const Edge<V, E>*) {
		return true;
	}
	void leaveVertex(const Vertex<V, E>*) {
		visited_.pop_back();
	}
	void leaveEdge(const Edge<V, E>*) {}
	const std::vector<const Vertex<V, E>*>& getVisited() const {
		return visited_;
	}

private:
	std::vector<const Vertex<V, E>*> visited_;
};

template<class V, class E, class C = std::equal_to<V>>
class VisitorPathBuilder : public VisitorOneTime<V, E> {
	using Parent = VisitorOneTime<V, E>;
	using Path = std::vector<const Vertex<V, E>*>;

public:
	explicit VisitorPathBuilder(const V& value, std::size_t pathCount = std::numeric_limits<std::size_t>::max())
		: value_(value), pathCount_(pathCount) {}

	bool visitVertex(const Vertex<V, E>* vertex) override {
		if (!Parent::visitVertex(vertex)) { return false; }
		if (C()(vertex->getProperties(), value_)) {
			pathes_->push_back(Parent::getVisited());
			Parent::leaveVertex(vertex);
			return false;
		}
		return true;
	}

	bool visitEdge(const Edge<V, E>* edge) override {
		if (!Parent::visitEdge(edge)) { return false; }
		if (pathes_->size() < pathCount_) { return true; }
		Parent::leaveEdge(edge);
		return false;
	}

	std::vector<Path>& getPathes() {
		return pathes_;
	}

private:
	const V value_;
	const std::size_t pathCount_;
	std::shared_ptr<std::vector<Path>> pathes_ = std::make_shared<std::vector<Path>>();
};

template<class V, class E, class C = std::equal_to<V>>
class VisitorPathCostBuilder : public VisitorPathBuilder<V, E, C> {
	using Parent = VisitorPathBuilder<V, E, C>;

public:
	explicit VisitorPathCostBuilder(const V& value, std::size_t pathCount = std::numeric_limits<std::size_t>::max())
		: Parent(value, std::numeric_limits<std::size_t>::max()), pathCount_(pathCount) {}

	bool visitVertex(const Vertex<V, E>* vertex) override {
		std::size_t oldPathesSize = Parent::getPathes().size();
		if (!Parent::visitVertex(vertex)) {
			if (Parent::getPathes().size() != oldPathesSize) {
				if (costs_->size() == pathCount_ && cost_ < (--costs_->end())->first) {
					auto lastItem = --costs_->end();
					Parent::getPathes().erase(Parent::getPathes().begin() + lastItem->second);
					costs_->erase(lastItem);
				}
				if (costs_->size() < pathCount_) { costs_->emplace(cost_, Parent::getPathes().size() - 1); }
			}
			return false;
		}
		if (costs_->size() == pathCount_ && cost_ >= (--costs_->end())->first) {
			Parent::leaveVertex(vertex);
			return false;
		}
		return true;
	}

	bool visitEdge(Edge<V, E>* edge) override {
		if (!Parent::visitEdge(edge)) { return false; }
		cost_ += edge->getProperties();
		return true;
	};
	void leaveEdge(Edge<V, E>* edge) {
		cost_ -= edge->getProperties();
	}

private:
	std::size_t pathCount_;
	int cost_ = 0;
	std::shared_ptr<std::multimap<int, int>> costs_ =
		std::make_shared<std::multimap<int, int>>();// cost, path position pairs
};

template<class V, class E>
class VisitorVertexCollector {
public:
	explicit VisitorVertexCollector(std::size_t deptLimit = std::numeric_limits<std::size_t>::max())
		: deptLimit_(deptLimit) {}

	VisitorVertexCollector(const VisitorVertexCollector& other) = default;

	virtual bool visitVertex(const Vertex<V, E>* vertex) {
		return vertices_.insert(vertex).second;
	}
	void leaveVertex(const Vertex<V, E>* /**/) {}
	bool visitEdge(const Edge<V, E>* /**/) {
		if (dept_ >= deptLimit_) { return false; }
		++dept_;
		return true;
	}
	void leaveEdge(const Edge<V, E>* /**/) {
		--dept_;
	}
	virtual ~VisitorVertexCollector() = default;

private:
	std::size_t deptLimit_;
	std::size_t dept_ = 0;
	std::unordered_set<const Vertex<V, E>*> vertices_;
};

template<class V, class E, class F>
class VisitorFuncApplyer : public VisitorVertexCollector<V, E> {
	using Parent = VisitorVertexCollector<V, E>;

public:
	explicit VisitorFuncApplyer(F f, std::size_t deptLimit = std::numeric_limits<std::size_t>::max())
		: Parent(deptLimit), _func(std::move(f)) {}
	bool visitVertex(const Vertex<V, E>* vertex) override {
		if (Parent::visitVertex(vertex)) {
			_func(vertex);
			return true;
		}
		return false;
	}

private:
	F _func;
};

template<class V, class E, class F, class Q>
void breadthPassCommon(const Vertex<V, E>* aVertex, F& visitor) {
	std::queue<const Vertex<V, E>*> vertices;
	vertices.push(aVertex);
	while (!vertices.empty()) {
		const Vertex<V, E>* current = vertices.front();
		vertices.pop();
		if (!visitor.visitVertex(current)) { continue; }
		for (auto* edge : current->getEdges()) {
			if (!visitor.visitEdge(edge)) { continue; }
			const Vertex<V, E>* next =
				edge->getVertex1() == current || edge->getVertex1() == nullptr ? edge->getVertex2() : edge->getVertex1();
			vertices.push(next);
		}
	}
}

template<class V, class E, class F>
void breadthPass(const Vertex<V, E>* vertex, F& visitor) {
	breadthPassCommon<V, E, F, std::queue<std::pair<const Vertex<V, E>*, F*>>>(vertex, visitor);
}

template<class V, class E, class F>
void breadthPassPriority(const Vertex<V, E>* vertex, F& visitor) {
	using QueueType = std::pair<const Vertex<V, E>*, F*>;
	struct PairLess {
		bool operator()(const QueueType& a, const QueueType& b) {
			return *a.second < *b.second;
		}
	};
	class FrontAdapter : public std::priority_queue<QueueType, std::vector<QueueType>, PairLess> {
	public:
		const QueueType& front() const {
			return std::priority_queue<QueueType, std::vector<QueueType>, PairLess>::top();
		}
	};
	breadthPassCommon<V, E, F, FrontAdapter>(vertex, &visitor);
}