#include <catch2/catch_test_macros.hpp>
#include "GatorBST.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

TEST_CASE("Basic Insertion and Search", "[basic]") {
    GatorBST tree;

    REQUIRE(tree.Height() == 0);

    REQUIRE(tree.Insert(50, "Root"));
    REQUIRE(tree.Height() == 1);

    auto result = tree.SearchID(50);
    REQUIRE(result.has_value());
    REQUIRE(result.value() == "Root");

    REQUIRE_FALSE(tree.SearchID(99).has_value());

    REQUIRE_FALSE(tree.Insert(50, "Duplicate"));
    
    REQUIRE(tree.Insert(30, "Left"));
    REQUIRE(tree.Insert(70, "Right"));
    REQUIRE(tree.Height() == 2);
}

TEST_CASE("Search By Name and Sorting", "[search]") {
    GatorBST tree;
    tree.Insert(10, "Alice");
    tree.Insert(5, "Alice");
    tree.Insert(20, "Alice");
    
    vector<int> results = tree.SearchName("Alice");
    
    REQUIRE(results.size() == 3);
    
    REQUIRE(results[0] == 5);
    REQUIRE(results[1] == 10);
    REQUIRE(results[2] == 20);
    
    REQUIRE(tree.SearchName("Bob").empty());
}

TEST_CASE("Remove Leaf and Single Child", "[remove]") {
    GatorBST tree;
    tree.Insert(50, "Root");
    tree.Insert(30, "Left");
    tree.Insert(70, "Right"); 
    tree.Insert(80, "RightRight");

    REQUIRE_FALSE(tree.Remove(999));

    REQUIRE(tree.Remove(30));
    REQUIRE_FALSE(tree.SearchID(30).has_value());
    
    vector<Node*> nodes = tree.TraverseInorder();
    REQUIRE(nodes.size() == 3);
    REQUIRE(nodes[0]->ufid == 50);

    REQUIRE(tree.Remove(70));
    REQUIRE_FALSE(tree.SearchID(70).has_value());
    
    nodes = tree.TraverseInorder();
    REQUIRE(nodes.size() == 2);
    REQUIRE(nodes[1]->ufid == 80);
}

TEST_CASE("Remove Node with Two Children - Successor Check", "[remove_successor]") {
    GatorBST tree;
    
    tree.Insert(50, "Root");
    tree.Insert(20, "Left");
    tree.Insert(70, "Right");
    tree.Insert(60, "Successor"); 
    tree.Insert(80, "RightRight");

    REQUIRE(tree.Remove(50));

    REQUIRE_FALSE(tree.SearchID(50).has_value());

    vector<Node*> pre = tree.TraversePreorder();
    REQUIRE(pre.size() == 4);
    
    REQUIRE(pre[0]->ufid == 60); 
    
    REQUIRE(pre[0]->right->ufid == 70);
    REQUIRE(pre[0]->right->left == nullptr);
}

TEST_CASE("Traversals Validation", "[traversal]") {
    GatorBST tree;
    tree.Insert(4, "D");
    tree.Insert(2, "B");
    tree.Insert(6, "F");
    tree.Insert(1, "A");
    tree.Insert(3, "C");
    tree.Insert(5, "E");
    tree.Insert(7, "G");

    vector<Node*> in = tree.TraverseInorder();
    REQUIRE(in.size() == 7);
    REQUIRE(in[0]->ufid == 1);
    REQUIRE(in[3]->ufid == 4);
    REQUIRE(in[6]->ufid == 7);

    REQUIRE(tree.Height() == 3);
    
    vector<Node*> pre = tree.TraversePreorder();
    REQUIRE(pre[0]->ufid == 4);
    REQUIRE(pre[1]->ufid == 2);
    REQUIRE(pre[4]->ufid == 6);
    
    vector<Node*> post = tree.TraversePostorder();
    REQUIRE(post[0]->ufid == 1);
    REQUIRE(post[6]->ufid == 4);
}

TEST_CASE("Edge Cases: Remove Root and Height Check", "[edge_remove]") {
    GatorBST tree;
    
    tree.Insert(10, "OnlyRoot");
    REQUIRE(tree.Height() == 1);
    
    REQUIRE(tree.Remove(10));
    
    REQUIRE(tree.Height() == 0); 
    REQUIRE_FALSE(tree.SearchID(10).has_value());
    REQUIRE(tree.TraverseInorder().empty());

    tree.Insert(20, "Root");
    tree.Insert(10, "LeftChild");
    
    REQUIRE(tree.Remove(20));
    
    auto rootCheck = tree.SearchID(10);
    REQUIRE(rootCheck.has_value());
    REQUIRE(tree.Height() == 1);
    
    tree.Remove(10);
    
    tree.Insert(20, "Root");
    tree.Insert(30, "RightChild");
    
    REQUIRE(tree.Remove(20));
    
    REQUIRE(tree.SearchID(30).has_value());
    REQUIRE(tree.Height() == 1);
}
