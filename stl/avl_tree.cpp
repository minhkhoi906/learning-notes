#include <iostream>
#include <memory>

using namespace std;

struct AVLNode {
    int key;
    int height;
    std::unique_ptr<AVLNode> left;
    std::unique_ptr<AVLNode> right;

    AVLNode(int k) : key(k), height(1) {}
};

class AVLTree
{
  private:
    std::unique_ptr<AVLNode> root;

    int height(const std::unique_ptr<AVLNode> &node) const
    {
        return node ? node->height : 0;
    }

    int getBalance(const std::unique_ptr<AVLNode> &node) const
    {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(AVLNode *node)
    {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    std::unique_ptr<AVLNode> rotateRight(std::unique_ptr<AVLNode> y)
    {
        std::unique_ptr<AVLNode> x = std::move(y->left);
        y->left = std::move(x->right);
        x->right = std::move(y);

        updateHeight(x->right.get());
        updateHeight(x.get());

        return x;
    }

    std::unique_ptr<AVLNode> rotateLeft(std::unique_ptr<AVLNode> x)
    {
        std::unique_ptr<AVLNode> y = std::move(x->right);
        x->right = std::move(y->left);
        y->left = std::move(x);

        updateHeight(y->left.get());
        updateHeight(y.get());

        return y;
    }

    std::unique_ptr<AVLNode> balance(std::unique_ptr<AVLNode> node)
    {
        updateHeight(node.get());

        int balance = getBalance(node);
        if (balance > 1) {
            if (getBalance(node->left) < 0) {
                node->left = rotateLeft(std::move(node->left));
            }
            return rotateRight(std::move(node));
        }
        if (balance < -1) {
            if (getBalance(node->right) > 0) {
                node->right = rotateRight(std::move(node->right));
            }
            return rotateLeft(std::move(node));
        }

        return node;
    }

    std::unique_ptr<AVLNode> insert(std::unique_ptr<AVLNode> node, int key)
    {
        if (!node)
            return std::make_unique<AVLNode>(key);

        if (key < node->key) {
            node->left = insert(std::move(node->left), key);
        } else if (key > node->key) {
            node->right = insert(std::move(node->right), key);
        } else {
            return node; // Duplicate keys are not allowed in BST
        }

        return balance(std::move(node));
    }

    void inorderTraversal(const std::unique_ptr<AVLNode> &node) const
    {
        if (!node)
            return;
        inorderTraversal(node->left);
        std::cout << node->key << " ";
        inorderTraversal(node->right);
    }

  public:
    void insert(int key) { root = insert(std::move(root), key); }

    void inorderTraversal() const
    {
        inorderTraversal(root);
        std::cout << std::endl;
    }
};

int main()
{
    AVLTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    std::cout << "Inorder traversal of the AVL tree is: ";
    tree.inorderTraversal();

    return 0;
}
