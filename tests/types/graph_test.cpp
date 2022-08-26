#include "types/graph.h"
#include <gtest/gtest.h>


TEST(GraphTest, DepthPassTest) {
	Vertex<std::string, int> a1("A1");
	Vertex<std::string, int> b1("B1");
	a1.addEdge(10, &b1);
	Vertex<std::string, int> c1("C1");
	b1.addEdge(20, &c1);
	Vertex<std::string, int> b2("B2");
	a1.addEdge(15, &b2);
	Vertex<std::string, int> e1("E1");
	c1.addOrderedEdge(30, &e1);
	Vertex<std::string, int> f1("F1");
	e1.addOrderedEdge(25, &f1);
	b2.addEdge(40, &f1);
	Vertex<std::string, int> b3("B3");
	a1.addOrderedEdge(10, &b3);
	b3.addEdge(12, &c1);

	std::vector<const Vertex<std::string, int>*> result;
	auto l = [&](const Vertex<std::string, int>* v) { result.push_back(v); };
	VisitorFuncApplyer<std::string, int, decltype(l)> visitor{ l };
	std::vector<Vertex<std::string, int>*> expected{ &a1, &b1, &c1, &e1, &f1, &b2, &b3 };
	depthPass(&a1, visitor);
	ASSERT_EQ(expected.size(), result.size()) << "Vectors x and y are of unequal length";

	for (std::size_t i = 0; i < expected.size(); ++i) {
		EXPECT_EQ(expected[i], result[i]) << "Vectors expected and visitor differ at index " << i;
	}
}

TEST(GraphTest, BreadthPassTest) {
	Vertex<std::string, int> a1("A1");
	Vertex<std::string, int> b1("B1");
	a1.addEdge(10, &b1);
	Vertex<std::string, int> c1("C1");
	b1.addEdge(20, &c1);
	Vertex<std::string, int> b2("B2");
	a1.addEdge(15, &b2);
	Vertex<std::string, int> e1("E1");
	c1.addOrderedEdge(30, &e1);
	Vertex<std::string, int> f1("F1");
	e1.addOrderedEdge(25, &f1);
	b2.addEdge(40, &f1);
	Vertex<std::string, int> b3("B3");
	a1.addOrderedEdge(10, &b3);
	b3.addEdge(12, &c1);

	std::vector<const Vertex<std::string, int>*> result;
	auto l = [&](const Vertex<std::string, int>* v) { result.push_back(v); };
	VisitorFuncApplyer<std::string, int, decltype(l)> visitor{ l };
	std::vector<Vertex<std::string, int>*> expected{ &a1, &b1, &b2, &b3, &c1, &f1, &e1 };
	breadthPass(&a1, visitor);
	ASSERT_EQ(expected.size(), result.size()) << "Vectors x and y are of unequal length";

	for (std::size_t i = 0; i < expected.size(); ++i) {
		EXPECT_EQ(expected[i], result[i]) << "Vectors expected and visitor differ at index " << i;
	}
}
