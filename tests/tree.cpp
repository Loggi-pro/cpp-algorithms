#include "algorithms/tree.h"
#include <algorithm>
#include <gtest/gtest.h>
// ALL PICTURES IS MIRRORED!!
// https://stackoverflow.com/questions/3955680/how-to-check-if-my-avl-tree-implementation-is-correct

TEST(TreeTest, Test1a) {
	/*
		20-   |      20
		\     |    /  \
		24    |=> 19    24
					|          \
					| 			    26
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(24);
	tree.insert(19);
	tree.insert(26);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 2;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, Test2a) {
	/*
		20- |      20
		\   |    /  \
		24  |=> 19    24
				|
				|
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(24);
	// std::cout << tree;
	tree.insert(19);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, Test5a) {
	/*
		20-   |       20-   |      24
			\   |         \   |    /  \
			24  |=>        24+|=> 20    23
					|          /  |
					|        23   |
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(24);
	// std::cout << tree;
	tree.insert(23);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, Test4a) {
	/*
		20- |       20-    |      24
		\   |         \    |    /  \
		24  |=>        24- |=> 20    26
				|            \ |
				|            26|
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(24);
	// std::cout << tree;
	tree.insert(26);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, Test3a) {
	/*
		20+  |       20++ |      4
	/      |     /      |    /  \
	4      |=>   4+     |=> 3    20
				 |   /        |
				 |  3         |
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(4);
	// std::cout << tree;
	tree.insert(3);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, Test6a) {
	/*
		20+  |       20++ |        20++ |     15
	/      |     /      |      /      |   /  \
	4      |=>   4-     |=>   15+     |=> 4    20
				 |     \      |   /		      |
				 |      15    |  4		      |
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(4);
	// std::cout << tree;
	tree.insert(15);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, Test6b) {
	/*
		20+     |       20++  |         20++  |       9
		 /  \   |      /  \   |        /  \   |      / \
		4    26 |=>   4-   26 |=>     9+   26 |=>   4+  20
	 / \      |    / \      |      / \      |    /   /  \
	3   9     |   3   9-    |     4+  15    |   3  15    26
						|		     \    |   /			      |
						|			    15  |  3		      	|
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(4);
	tree.insert(26);
	tree.insert(9);
	tree.insert(3);
	// std::cout << tree;
	tree.insert(15);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 2;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, Test6c) {
	/*
			__20+__         |         _20++_       |           __20++_       |         ___9___
		 /       \        |        /      \      |          /       \      |        /       \
		4         26      |=>     4-       26    |=>       9+        26    |=>     4+      __20__
		 / \       /  \   |      / \      /  \   |        / \       /  \   |      / \     /      \
		3+  9    21    30 |     3+  9-  21    30 |       4+  11-  21    30 |     3+  7  11-       26
	 /   / \            |    /   / \           |      / \   \            |    /         \      /  \
	2   7   11          |   2   7   11-        |     3+  7   15          |   2           15  21    30
											|			 \               |   /					           |
											|			  15             |  2						           |
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(4);
	tree.insert(26);
	tree.insert(3);
	tree.insert(9);
	tree.insert(21);
	tree.insert(30);
	tree.insert(2);
	tree.insert(7);
	tree.insert(11);
	// std::cout << tree;
	tree.insert(15);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 3;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, Test6a2) {
	/*
		20+ |     20++ |       20++ |     8
	 /    |    /     |      /     |    / \
	4     |=> 4-     |=>   8+     |=> 4   20
				|	  \      |   /		    |
				|	   8     |  4		      |
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(4);
	// std::cout << tree;
	tree.insert(8);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, Test6b2) {
	/*
		20+           20++           20++         9
		 /  \         /  \           /  \         / \
		4    26 =>   4-   26 =>     9++  26 =>   4   20-
	 / \          / \            /            / \    \
	3   9        3   9+         4            3   8    26
							/              / \
						 8              3   8
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(4);
	tree.insert(26);
	tree.insert(9);
	tree.insert(3);
	// std::cout << tree;
	tree.insert(8);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 2;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, Test6c2) {
	/*
			__20+__                  _20++_                  __20++_                ___9___
		 /       \                /      \                /       \              /       \
		4         26             4-       26             9+        26           4        _20-
		 / \       /  \         / \      /  \           / \       /  \         / \      /    \
		3+  9    21    30 =>   3+  9+  21    30 =>     4   11   21    30 =>   3+  7-  11      26
	 /   / \                /   / \                 / \                    /     \         /  \
	2   7   11             2   7-  11              3+  7-                 2       8      21    30
						\                 /     \
						 8               2       8
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(20);
	tree.insert(4);
	tree.insert(26);
	tree.insert(3);
	tree.insert(9);
	tree.insert(21);
	tree.insert(30);
	tree.insert(2);
	tree.insert(7);
	tree.insert(11);
	// std::cout << tree;
	tree.insert(8);
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 3;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, TestDelete_Base_1R) {
	/*Remove right node of right subtree
	 * Loop, ancestor have balance "=" - loop ends;
	 */
	/*
		4             4
	 / \           / \
	2   6=   ==>   2   6
		 / x
		5	  8
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(4);
	tree.insert(2);
	tree.insert(6);
	tree.insert(8);
	tree.erase(tree.find(8));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, TestDelete_Base_2R) {
	/*Remove right node of subtree
	Enable loop. Ancestor have balance "R" -  node-ancestor replaced by ancesor->parent. Move up to tree
	*/
	/*
		4             4
	 / \           / \
	2   6R   ==>   2   6
			 x
				8
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(4);
	tree.insert(2);
	tree.insert(6);
	tree.insert(8);
	tree.erase(tree.find(8));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, TestDelete_Base_3R) {
	/*Remove right node of subtree
	Enable loop. Ancestor have balance "L" - there are 3 variants of balance for left node
	ancestor->left.balanceFactor='='  Right rotate
	*/
	/*
			5L                  2
		 /  x                / \
		2=   6   =>R        1   5
	 / \				          	/  \
	1   4			             	   4
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(5);
	tree.insert(2);
	tree.insert(6);
	tree.insert(1);
	tree.insert(4);
	tree.erase(tree.find(6));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 2;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, DISABLED_TestDelete_Base_4R) {
	/*Remove right node of subtree
	Enable loop. Ancestor have balance "L" -  there are 3 variants of balance for left node:
	ancestor->left.balanceFactor='L'  Move down to tree. If last node is=L then do right rotate
	*/
	/*
		 5L                  2
		/  x                / \
	 2L   6   =>R        1   5
	 /
	1
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(5);
	tree.insert(2);
	tree.insert(6);
	tree.insert(1);
	tree.erase(tree.find(6));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, DISABLED_TestDelete_Base_5R) {
	/*Remove right node subtree
	Enable loop. Ancestor have balance "L" - there are 3 variants of balance for left node
	ancestor->left.balanceFactor='R'  Make left rotate and then right
	*/
	/*
		5L                 5L       4
	 / x                / x      / \
	2R   6   == L ==> 4    6 =>R 2   5
	 \				  /
		4				  2
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(5);
	tree.insert(2);
	tree.insert(6);
	tree.insert(4);
	tree.erase(tree.find(6));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, TestDelete_Case_1L) {
	/*Remove left node subtree
	Enable loop. Ancestor have balance "=" - loop ends
	*/
	/*
		2          2            4
	 x \          \          / \
	1   4    =>    4    =>  2   5
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(2);
	tree.insert(1);
	tree.insert(4);
	tree.insert(3);
	tree.insert(5);
	tree.erase(tree.find(1));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 2;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, TestDelet_Base_2L) {
	/*Remove left node subtree
	Enable loop. Ancestor have balance "L" - Ancesor-node replaces by ancesor->parent. Move up to tree
	*/
	/*
			6R
		 / \
		4L   8
	 /   /  \
	2L  7    10==>
	 x	      \
	1		       12
								\
				14
 */
	/*
			6=		  	   6=
			/ \		  	  / \
			 4L  8	   4L  8
			/       ==>   /
		2L  		       2L
		x
		 1
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(6);
	tree.insert(4);
	tree.insert(8);
	tree.insert(2);
	tree.insert(1);
	tree.erase(tree.find(1));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 2;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, TestDelete_Case_3L) {
	/*Remove left node subtree
	Enable loop. Ancestor have balance "L" - There are 3 variants of balance for left node
	ancestor->left.balanceFactor='='  Make left rotate
	 */
	/*
		2R          2            4
	 x \           \          / \
	1   4=    =>    4    =>  2   5
		 / \         / \        \
		3   5       3   5        3
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(2);
	tree.insert(1);
	tree.insert(4);
	tree.insert(3);
	tree.insert(5);
	tree.erase(tree.find(1));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 2;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, TestDelete_Base_4L) {
	/*Удаление левого узла  поддерева
	Enable loop. Ancestor have balance "R" - There are 3 variants of balance for left node
	ancestor->left.balanceFactor='R'  move down to tree, if last node=R then make left rotate
	*/
	/*
		4R                  5
	 x \                 / \
	2   5R   == 1L ==>  4   6
			 \
				6
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(4);
	tree.insert(2);
	tree.insert(5);
	tree.insert(6);
	tree.erase(tree.find(2));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, DISABLED_TestDelete_Base_5L) {
	/*Удаление правого узла  поддерева
	Enable loop. Ancestor have balance "R" - There are 3 variants of balance for right node:
	ancestor->left.balanceFactor='L'  Make right rotate and then left
	 */
	/*
		4R                  5
	 x \                / \
	2   6L   == 2L ==>  4   6
		 /
		5
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(4);
	tree.insert(2);
	tree.insert(6);
	tree.insert(5);
	tree.erase(tree.find(2));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 1;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, TestDelete_ComplexOpA) {
	/*
			6                 10
		 / \               / \
		4   10  == 1R ==> 6   12
		 x   / \           / \   \
		2   8   12         4   8   14
				 \
				 14
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(6);
	tree.insert(4);
	tree.insert(10);
	tree.insert(2);
	tree.insert(8);
	tree.insert(12);
	tree.insert(14);
	tree.erase(tree.find(2));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 2;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, DISABLED_TestDelete_ComplexOpB) {
	/*
			- 10 -                 6
		 /     \                / \
		6       14  == 1R ==>  4   10
		 / \     x              /   / \
		4   8   12             2   8   14
	 /
	2
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(10);
	tree.insert(6);
	tree.insert(14);
	tree.insert(4);
	tree.insert(8);
	tree.insert(12);
	tree.insert(2);
	tree.erase(tree.find(12));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 2;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, DISABLED_TestDelete_ComplexOpC) {
	/*
		- 10-                       —-16 —-
		 /     \                     /       \
		6       20                  -10-      20
	 / \     / \   == 2L ==>     /    \    / \
	2R   8  16   22              6     14  18  22
	 x     / \   \             / \    /        \
		4  14  18   24          2  8  12         24
		 /
		12
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(10);
	tree.insert(6);
	tree.insert(20);
	tree.insert(2);
	tree.insert(8);
	tree.insert(16);
	tree.insert(22);
	tree.insert(4);
	tree.insert(14);
	tree.insert(18);
	tree.insert(24);
	tree.insert(12);
	tree.erase(tree.find(4));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 3;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, DISABLED_TestDelete_ComplexOpD) {
	/*
			-16 -                    -10-
		 /     \                  /     \
		6       22               6       - 16 -
		 / \     / \  == 2R ==>   / \     /     \
		4  10   18  24           4   8   12      22
	 /   / \   x              /         \     / \
	2   8   12   20           2          14  18   24
			 \
				14
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(16);
	tree.insert(6);
	tree.insert(22);
	tree.insert(4);
	tree.insert(10);
	tree.insert(18);
	tree.insert(24);
	tree.insert(2);
	tree.insert(8);
	tree.insert(12);
	tree.insert(20);
	tree.insert(14);
	tree.erase(tree.find(20));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 3;
	EXPECT_EQ(true, res);
}


TEST(TreeTest, TestDelete_CaseB) {
	/*
		___6___                ___6___                 ___6___
		 /       \              /       \               /       \
		2         9            2         9             4         9
	 x \       / \            \       / \           / \       / \
	1   4     8  12     =>     4     8  12      => 2   5     8   12
		 / \   /   / \          / \   /   / \         \       /   / \
		3   5 7   10  13       3   5 7  10  13        3      7  10   13
					 \                      \                       \
					 14                     14                      14
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(6);
	tree.insert(2);
	tree.insert(9);
	tree.insert(1);
	tree.insert(4);
	tree.insert(8);
	tree.insert(12);
	tree.insert(3);
	tree.insert(5);
	tree.insert(7);
	tree.insert(10);
	tree.insert(13);
	tree.insert(14);
	tree.erase(tree.find(1));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 4;
	EXPECT_EQ(true, res);
}
TEST(TreeTest, DISABLED_TestDelete_CaseC) {
	/*
		___5___              ___5___                 ___5___                   ____8____
		 /       \            /       \               /       \                 /         \
		2         8          2         8             3         8              _5_         10
	 x \       / \          \       / \           / \       / \            /   \        / \
	1   3R     7  10     => 3     7   10     =>  2   4     7  10     =>   3     7      9   12
		 \   /   / \          \   /   / \                 /   / \        / \   /            \
			4 6   9  12          4 6   9  12               6   9  12      2   4 6              13
					 \                    \                       \
					 13                    13                      13
	*/
	AVLTree<int, std::greater<int>> tree;
	tree.insert(5);
	tree.insert(2);
	tree.insert(8);
	tree.insert(1);
	tree.insert(3);
	tree.insert(7);
	tree.insert(10);
	tree.insert(4);
	tree.insert(6);
	tree.insert(9);
	tree.insert(12);
	tree.insert(13);
	tree.erase(tree.find(1));
	// std::cout << tree;
	int height = tree.height();
	bool res = height == 4;
	EXPECT_EQ(true, res);
}

TEST(TreeTest, TestThatAVLTreeAlwaysBalanced) {
	AVLTree<int, std::greater<int>> avltree;
	size_t n = 800;

	for (size_t i = 0; i < n; i++) {
		int a = rand();
		avltree.insert(a);

		EXPECT_EQ(true, avltree.isAVLTree());
	}
}