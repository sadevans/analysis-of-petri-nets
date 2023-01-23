#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "graph_translators.hpp"
#include "automation.hpp"

using namespace std;

//Класс узла
class Node{
public:
    map<string, int> data;									//словарь маркировок
    Node* parent;											//родительский узел
    Node(map<string, int> d) { parent = NULL; data = d; };	//конструктор, создающий начальный узел
    Node(Node* par, map<string, int> d);					//конструктор, создающий новый узел по родителю и маркировке
    Node() {};												//конструктор по умолчанию
};

//Класс анализатора
class Analyser {
	private:
		int term = 0;					//счетчик терминальных вершин
		set< map<string, int> > close;	//множество закрытых вершин
		set< map<string, int> > open;	//множество открытых вершин
		int dubl_start = 0;				//счетчик вершин, дублирующих начальную
		set <string> done_events;		//множество выполненных переходов
		map<int, Node> tree;			//дерево

	public:
		Analyser() {};											//конструктор по умолчанию
		void analyse_node(Node* start, des::Automation& model);	//метод обработки вершины дерева
		map<string, bool> run_analyse(des::Automation& model);	//метод анализа сети Петри
		int bfs(des::Automation& model);						//метод анализа сети на связность
};