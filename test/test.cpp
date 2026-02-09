#include <catch2/catch_test_macros.hpp>
#include "GatorBST.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

TEST_CASE("Basic Operations and Empty Tree", "[basic]") {
    GatorBST tree;

    REQUIRE(tree.Height() == 0);
    REQUIRE(tree.TraverseInorder().empty());
    REQUIRE(tree.TraversePreorder().empty());
    REQUIRE(tree.TraversePostorder().empty());
    REQUIRE(tree.SearchName("Alice").empty());
    REQUIRE_FALSE(tree.SearchID(1).has_value());
    REQUIRE_FALSE(tree.Remove(1));

    REQUIRE(tree.Insert(50, "Root"));
    REQUIRE(tree.Height() == 1);
    
    REQUIRE_FALSE(tree.Insert(50, "Duplicate"));
    
    REQUIRE(tree.Insert(30, "Left"));
    REQUIRE(tree.Insert(70, "Right"));
    REQUIRE(tree.Height() == 2);

    auto result = tree.SearchID(50);
    REQUIRE(result.has_value());
    REQUIRE(result.value() == "Root");
}

TEST_CASE("SearchName Sorting", "[search]") {
    GatorBST tree;
    tree.Insert(10, "A");
    tree.Insert(5, "A");
    tree.Insert(20, "A");
    
    vector<int> results = tree.SearchName("A");
    REQUIRE(results.size() == 3);
    
    REQUIRE(results[0] == 5);
    REQUIRE(results[1] == 10);
    REQUIRE(results[2] == 20);
    
    REQUIRE(tree.SearchName("B").empty());
}

TEST_CASE("Remove Leaf and Single Child", "[remove_simple]") {
    GatorBST tree;
    tree.Insert(50, "Root");
    tree.Insert(30, "L");
    tree.Insert(70, "R"); 
    tree.Insert(80, "RR");

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

TEST_CASE("Remove Root Edge Cases", "[remove_root]") {
    GatorBST tree;
    
    tree.Insert(10, "OnlyRoot");
    REQUIRE(tree.Remove(10));
    REQUIRE(tree.Height() == 0); 
    
    tree.Insert(20, "Root");
    tree.Insert(10, "Left");
    REQUIRE(tree.Remove(20));
    REQUIRE(tree.Height() == 1);
    REQUIRE(tree.SearchID(10).has_value());
    tree.Remove(10);
    
    tree.Insert(20, "Root");
    tree.Insert(30, "Right");
    REQUIRE(tree.Remove(20));
    REQUIRE(tree.Height() == 1);
    REQUIRE(tree.SearchID(30).has_value());
}

TEST_CASE("Remove Two Children Successor Logic", "[remove_complex]") {
    GatorBST tree;

    tree.Insert(50, "Root");
    tree.Insert(20, "L");
    tree.Insert(70, "R");
    tree.Insert(60, "Succ"); 
    tree.Insert(65, "SuccChild"); 
    tree.Insert(80, "RR");

    REQUIRE(tree.Remove(50));
    REQUIRE_FALSE(tree.SearchID(50).has_value());

    vector<Node*> pre = tree.TraversePreorder();
    REQUIRE(pre.size() == 5);
    
    REQUIRE(pre[0]->ufid == 60);       
    REQUIRE(pre[0]->left->ufid == 20); 
    REQUIRE(pre[0]->right->ufid == 70);
    
    REQUIRE(pre[0]->right->left->ufid == 65); 
    REQUIRE(pre[0]->right->right->ufid == 80);
}

TEST_CASE("Remove Two Children Immediate Successor", "[remove_immediate]") {
    GatorBST tree;
    tree.Insert(50, "Root");
    tree.Insert(20, "L");
    tree.Insert(80, "R_Succ");
    tree.Insert(90, "RR");

    REQUIRE(tree.Remove(50));
    
    vector<Node*> pre = tree.TraversePreorder();
    REQUIRE(pre.size() == 3);
    REQUIRE(pre[0]->ufid == 80);
    REQUIRE(pre[0]->left->ufid == 20);
    REQUIRE(pre[0]->right->ufid == 90);
}

TEST_CASE("Traversals Structure", "[traversal]") {
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
    REQUIRE(in[6]->ufid == 7); 

    vector<Node*> pre = tree.TraversePreorder();
    REQUIRE(pre[0]->ufid == 4); 
    REQUIRE(pre[1]->ufid == 2); 

    vector<Node*> post = tree.TraversePostorder();
    REQUIRE(post[0]->ufid == 1); 
    REQUIRE(post[6]->ufid == 4); 
}
