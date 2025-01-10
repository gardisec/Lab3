#define BOOST_TEST_MODULE TreeTests

#include <boost/test/included/unit_test.hpp>

#include "classes/ComplTree.h"

BOOST_AUTO_TEST_CASE(test_create_tree) {
    Tree* root = new Tree("root");

    BOOST_CHECK(root != nullptr);
    BOOST_CHECK(root->getKey() == "root");
    BOOST_CHECK(root->getLeft() == nullptr);
    BOOST_CHECK(root->getRight() == nullptr);

    delete root;
}

BOOST_AUTO_TEST_CASE(test_insert_to_tree) {
    Tree* root = new Tree("root");

    root->insertToTree("left");
    root->insertToTree("right");

    BOOST_CHECK(root->getLeft() != nullptr);
    BOOST_CHECK(root->getRight() != nullptr);
    BOOST_CHECK(root->getLeft()->getKey() == "left");
    BOOST_CHECK(root->getRight()->getKey() == "right");

    root->insertToTree("left_left");
    root->insertToTree("left_right");

    BOOST_CHECK(root->getLeft()->getLeft() != nullptr);
    BOOST_CHECK(root->getLeft()->getRight() != nullptr);
    BOOST_CHECK(root->getLeft()->getLeft()->getKey() == "left_left");
    BOOST_CHECK(root->getLeft()->getRight()->getKey() == "left_right");

    delete root;
}

BOOST_AUTO_TEST_CASE(test_is_complete_binary_tree) {
    Tree* root = new Tree("root");

    root->insertToTree("left");
    root->insertToTree("right");
    root->insertToTree("left_left");
    root->insertToTree("left_right");
    root->insertToTree("right_left");

    BOOST_CHECK(Tree::isCompleteBinaryTree(root) == true);

    root->getLeft()->setLeft(nullptr);

    delete root;
}

BOOST_AUTO_TEST_CASE(test_search) {
    Tree* root = new Tree("root");

    root->insertToTree("left");
    root->insertToTree("right");
    root->insertToTree("left_left");
    root->insertToTree("left_right");

    BOOST_CHECK(Tree::search(root, "left") == true);
    BOOST_CHECK(Tree::search(root, "right") == true);
    BOOST_CHECK(Tree::search(root, "left_left") == true);
    BOOST_CHECK(Tree::search(root, "non_existent") == false);

    delete root;
}

BOOST_AUTO_TEST_CASE(test_operator_output) {
    Tree* root = new Tree("root");

    root->insertToTree("left");
    root->insertToTree("right");
    root->insertToTree("left_left");
    root->insertToTree("left_right");

    std::stringstream ss;
    ss << *root;
    BOOST_CHECK(ss.str() == "root left right left_left left_right ");

    delete root;
}

BOOST_AUTO_TEST_CASE(test_delete_tree) {
    Tree* root = new Tree("root");

    root->insertToTree("left");
    root->insertToTree("right");
    root->insertToTree("left_left");
    root->insertToTree("left_right");

    delete root;

    BOOST_CHECK(true);  //если память была освобождена корректно, тест пройдет
}

BOOST_AUTO_TEST_CASE(test_empty_tree_search) {
    Tree* root = new Tree("root");

    BOOST_CHECK(Tree::search(root, "root") == true);
    BOOST_CHECK(Tree::search(root, "non_existent") == false);

    delete root;
}
