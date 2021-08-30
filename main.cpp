#include <iostream>
#include <queue>
#include <vector>

struct Node {
  std::vector<int> point;
  std::unique_ptr<Node> childs[2];
};

double distance(const std::vector<int>& p1, const std::vector<int>& p2) {
  return (p1[0] - p2[0]) * (p1[0] - p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1]);
}
double distance(Node* p1, Node* p2) { return distance(p1->point, p2->point); }
double distance(const std::vector<int>& p1, Node* p2) { return distance(p1, p2->point); }

std::unique_ptr<Node> buildTree(std::vector<std::vector<int>> points, int depth) {
  if (points.empty()) {
    return nullptr;
  }

  const int k = depth % 2;
  auto p = std::move(points);
  int mid_idx = p.size() / 2;
  std::nth_element(
      p.begin(), p.begin() + mid_idx, p.end(),
      [k](const std::vector<int>& l, const std::vector<int>& r) { return l[k] < r[k]; });

  auto n = std::make_unique<Node>();
  n->point = p[p.size() / 2];
  if (p.size() > 1) {
    n->childs[0] = buildTree(std::vector(p.begin(), p.begin() + mid_idx), depth + 1);
  }
  if (p.size() > 2) {
    n->childs[1] = buildTree(std::vector(p.begin() + mid_idx + 1, p.end()), depth + 1);
  }

  return n;
}

Node* findNN(Node* root, const std::vector<int>& i, int d) {
  if (!root) {
    return nullptr;
  }

  int k = d % 2;
  Node* min = root;
  Node* lmin = nullptr;
  if (i[k] < root->point[k] || (i[k] - root->point[k]) < distance(i, min)) {
    if (root->childs[0]) {
      lmin = findNN(root->childs[0].get(), i, d + 1);
    }
  }
  if (lmin && distance(i, lmin) < distance(i, min)) {
    min = lmin;
  }

  Node* rmin = nullptr;
  if (i[k] > root->point[k] || (root->point[k] - i[k]) < distance(i, min)) {
    if (root->childs[1]) {
      rmin = findNN(root->childs[1].get(), i, d + 1);
    }
  }
  if (rmin && distance(i, rmin) < distance(i, min)) {
    min = rmin;
  }

  return min;
}

void printTree(Node* root) {
  std::queue<Node*> q;
  q.push(root);
  while (!q.empty()) {
    auto* n = q.front();
    if (n) {
      std::cout << '(' << n->point[0] << ',' << n->point[1] << ')';
      if (n->childs[0]) {
        q.push(n->childs[0].get());
      } else {
        q.push(nullptr);
      }
      if (n->childs[1]) {
        q.push(n->childs[1].get());
      } else {
        q.push(nullptr);
      }
    } else {
      std::cout << "(Null)";
    }
    q.pop();
  }
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  auto points =
      std::vector<std::vector<int>>{{2, 3}, {5, 4}, {9, 6}, {4, 7}, {8, 1}, {7, 2}, {10, 2}};
  auto root = buildTree(points, 0);
  printTree(root.get());

  auto nn = findNN(root.get(), std::vector<int>{7, 0}, 0);
  std::cout << "NN: (" << nn->point[0] << "," << nn->point[1] << ")" << std::endl;

  nn = findNN(root.get(), std::vector<int>{0, 0}, 0);
  std::cout << "NN: (" << nn->point[0] << "," << nn->point[1] << ")" << std::endl;

  nn = findNN(root.get(), std::vector<int>{10, 10}, 0);
  std::cout << "NN: (" << nn->point[0] << "," << nn->point[1] << ")" << std::endl;

  return 0;
}