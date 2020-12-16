#pragma once
#include <cstdint>

#define OFFSETOF(s, m) (std::size_t) & (((s *)0)->m)
template<class T, class Allocator>
struct ListNode {
	using PListNode = typename Allocator::template SmartPointer<ListNode>;
	PListNode next;
	PListNode prev;
	T data;
	ListNode() = default;
};

template<class T>
class ListAllocatorRAM {
public:
	template<class D>
	struct SmartPointer {
		std::uintptr_t addr = 0;
		D &operator*() const {
			return *(D *)addr;
		}
		// drill - down behavior."
		// https://stackoverflow.com/questions/8777845/overloading-member-access-operators-c
		T *operator->() const {
			return (D *)addr;
		}
		SmartPointer<D> &operator=(const SmartPointer<D> &other) {
			addr = other.addr;
			return *this;
		}

		bool operator==(const SmartPointer<D> &other) const {
			return addr == other.addr;
		}
		bool operator!=(const SmartPointer<D> &other) const {
			return !operator==(other);
		}

		SmartPointer(const SmartPointer<D> &other) : addr(other.addr) {}


		SmartPointer() : addr(0) {}
	};
	using PListNode = SmartPointer<T>;

private:
	PListNode next_avail;
	PListNode last;//последний свободный блок
	PListNode free_List;//список свободных блоков

	struct ListNodeBuffer {//список буферов
		ListNodeBuffer *next_buffer;
		PListNode buffer;
	};
	ListNodeBuffer *buffer_List;//указатель на первый буффер
	void _addBuffer(ListNodeBuffer **buffer) {
		enum { BUFFER_SIZE = 10 };
		(*buffer) = new ListNodeBuffer();
		T *arr = new T[BUFFER_SIZE];
		(*buffer)->buffer.addr = (std::uintptr_t)&arr[0];
		(*buffer)->next_buffer = nullptr;
		next_avail.addr = (std::uintptr_t)&arr[0];
		last.addr = (std::uintptr_t)&arr[BUFFER_SIZE - 1];
	}

public:
	ListAllocatorRAM() {
		_addBuffer(&buffer_List);
		free_List.addr = 0;
	}


	template<typename T2>
	SmartPointer<T2> getMemory() {
		T2 *obj = nullptr;

		if (free_List.addr != 0) {//смотрим список свободных блоков
			obj = new ((T2 *)free_List.addr) T2();
			free_List = (*free_List).next;
		} else {//берем из буфера
			obj = new ((T2 *)next_avail.addr) T2();
			next_avail.addr += sizeof(T2);

			if (next_avail == last) {//если в буфере нет места,
				//создаем следующий буфер
				ListNodeBuffer *curBuffer = buffer_List;
				while (curBuffer->next_buffer != nullptr) { curBuffer = curBuffer->next_buffer; }

				_addBuffer(&curBuffer);
			}
		}

		SmartPointer<T> p;
		p.addr = (std::uintptr_t)obj;
		return p;
	}
	template<typename T2>
	std::uintptr_t addressOf(SmartPointer<T2> &p) {
		return p.addr;
	}
	template<typename T2>
	void construct(std::uintptr_t address, const T2 &o) {
		T2 *obj = new ((T2 *)address) T2(o);
		(void)obj;
	}
	template<typename T2>
	void putMemory(SmartPointer<T2> &p) {
		T2 *ptr = (T2 *)p.addr;
		ptr->~T2();
		ptr->next = free_List;
		free_List = p;
	}
};


template<typename T>
class List {
private:
	std::size_t _length = 0;
	struct ListNode {
		using PListNode = typename ListAllocatorRAM<ListNode>::template SmartPointer<ListNode>;
		PListNode prev;
		PListNode next;
		T data;
	};
	using Allocator = ListAllocatorRAM<ListNode>;
	Allocator _alloc;
	using PListNode = typename ListNode::PListNode;
	PListNode node;

public:
	struct iterator {
	protected:
		friend class List<T>;
		PListNode _item;
		explicit iterator(PListNode node) : _item(node) {}

	public:
		iterator(const iterator &other) = default;
		iterator &operator++() {
			_item = (*_item).next;
			return *this;
		}
		iterator operator++(int) {
			iterator tmp(*this);
			_item = (*_item).next;
			return tmp;
		}
		iterator &operator--() {
			_item = (*_item).prev;
			return *this;
		}
		iterator operator--(int) {
			iterator tmp(*this);
			_item = (*_item).prev;
			return tmp;
		}
		T &operator*() {
			return (*_item).data;
		}
		bool operator==(const iterator &x) {
			return _item == x._item;
		}
		bool operator!=(const iterator &x) {
			return !operator==(x);
		}
	};
	List() {
		// get node
		// node = get_node();
		node = _alloc.template getMemory<ListNode>();
		(*node).next = node;
		(*node).prev = node;
	}

	void push_back(const T &x) {
		iterator tmp = end();
		insert(tmp, x);
	}
	void push_front(const T &x) {
		insert(begin(), x);
	}
	iterator insert(iterator &position, const T &x) {
		// get node
		PListNode tmp(_alloc.template getMemory<ListNode>());
		_alloc.construct(_alloc.addressOf(tmp) + OFFSETOF(ListNode, data), x);
		(*tmp).next = position._item;
		(*tmp).prev = (*position._item).prev;
		(*(*position._item).prev).next = tmp;
		(*position._item).prev = tmp;
		_length++;
		return iterator(tmp);
	}
	void pop_front() {
		erase(begin());
	}

	void pop_back() {
		iterator tmp = end();
		erase(--tmp);
	}
	void erase(iterator &position) {
		(*(*position._item).prev).next = (*position._item).next;
		(*(*position._item).next).prev = (*position._item).prev;
		_alloc.template putMemory<ListNode>(position._item);
		--_length;
	}

	std::size_t size() const {
		return _length;
	}

	bool empty() const {
		return _length == 0;
	}
	iterator begin() {
		return iterator((*node).next);
	}
	iterator end() {
		return iterator(node);
	}

	T &back() {
		iterator tmp = end();
		return *(--tmp);
	}
	const T &back() const {
		iterator tmp = end();
		return *(--tmp);
	}
};
