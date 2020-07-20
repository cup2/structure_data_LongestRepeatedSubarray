#include <string>
#include <map>
#include <vector>
#include <iostream>

const int SIZE_ALPHABET = 255;

struct node {
	int position, length;
	int parent;
	std::map<int, int> child;
	std::map<int, int> link;
};

void attach(const int& child, const int& parent, const int& c, const int& child_len, std::vector<node>& tree) {
	tree[parent].child[c] = child;
	tree[child].length = child_len;
	tree[child].parent = parent;
}

void extend(const int& i, std::vector<int>& s, std::vector<node>& tree, int& sz, int* path) {
	int v, vlen = s.size() - i, old = sz - 1, pstk = 0;
	for (v = old; !tree[v].link.count(s[i]); v = tree[v].parent) {
		vlen -= tree[v].length;
		path[pstk++] = v;
	}
	int w = tree[v].link[s[i]];
	if (tree[w].child.count(s[i + vlen])) {
		int u = tree[w].child[s[i + vlen]];
		for (tree[sz].position = tree[u].position - tree[u].length; s[tree[sz].position] == s[i + vlen]; tree[sz].position += tree[v].length) {
			v = path[--pstk];
			vlen += tree[v].length;
		}
		attach(sz, w, s[tree[u].position - tree[u].length], tree[u].length - (tree[u].position - tree[sz].position), tree);
		attach(u, sz, s[tree[sz].position], tree[u].position - tree[sz].position, tree);
		w = tree[v].link[s[i]] = sz++;
	}
	tree[old].link[s[i]] = sz;
	attach(sz, w, s[i + vlen], s.size() - (i + vlen), tree);
	tree[sz++].position = s.size();
}

void initTree(std::vector<node>& tree) {
	tree[1].length = 1; tree[1].position = 0; tree[1].parent = 0;
	for (int ch = 1; ch <= SIZE_ALPHABET; ch++) {
		tree[0].link[ch] = 1;
	}
}

void longestRepeatedSubarrayRekur(const std::vector<node>& tree, const int& nd, const int& labelHeight, int& max) {
	for (const auto& it : tree[nd].child) {
		if (tree[it.second].child.size() != 0) {
			longestRepeatedSubarrayRekur(tree, it.second, labelHeight + tree[it.second].length, max);
		}
		else {
			max = (max < labelHeight ? labelHeight : max);
		}
	}
}

int getLengthLongestRepeatedSubstring(const std::vector<node>& tree) {
	int maxHeight = 0;
	longestRepeatedSubarrayRekur(tree, 1, 0, maxHeight);
	return maxHeight;
}

int main() {
	std::vector<int> arr;
	while (true) {
		int tmp;
		std::cin >> tmp;
		if (tmp == 0) {
			break;
		}
		arr.push_back(tmp);
	}
	std::vector<node> tree(arr.size() * 2 + 1);
	initTree(tree);
	int sz = 2;
	int* path = new int[tree.size()];
	for (int i = arr.size() - 1; i >= 0; i--) {
		extend(i, arr, tree, sz, path);
	}
	std::cout << getLengthLongestRepeatedSubstring(tree);
}