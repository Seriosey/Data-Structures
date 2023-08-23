#include <cstdlib>
#include <iostream>
#include <cmath>

namespace itis {
	using namespace std;

	struct node {
		node* parent;
		node* child;
		node* left;
		node* right;
		int key;
		int degree = 0;
	};

	struct fibbHeap {
		node* minHeapNode = NULL;
		int nodeNumb = 0;

	public:
		void insert(int val);

		node findMin() { return *minHeapNode; }

		void linkParentWithChild(node* ptr2, node* ptr1);

		void extractMin();

	private:
		void consolidate();
	};
}  // namespace itis