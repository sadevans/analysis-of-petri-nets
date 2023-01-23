#include <typeinfo>
#include "analyse.hpp"
using namespace std;

set<string> getPotReadyEvents(des::Automation& model, map <string, int> mark){
    
    set<string> result = {};							//множество, в котором будут храниться переходы, готовые сработать
    
    for (const auto& e : model.getEventNameSet()) {		//Для всех переходов
        bool is_ready = false;							//По умолчанию присваиваем переменной is_ready значение ложь.
        for (const auto& s : model.getEventInputs(e)) {	//Для всех элементов из множества состояний для данного перехода
            const auto& multiplicity = mark[s];			//Переменной кратности присваиваем маркировку данного состояния.

            if (multiplicity >= model.getLinksFromStateToEvent(s, e) || multiplicity == -1) {	//Если кратность не меньше количества ссылок из состояния к переходу ИЛИ кратность равна -1
                is_ready = true;	//Переопределяем значение переменной is_ready на правду
            }
            else {					//Иначе
                is_ready = false;	//Значение переменной is_ready - ложь
                break;
            }						//Прерываем выполнение.
        }
        if (is_ready) {				//Если в переменной is_ready хранится значение правда
            result.insert(e);		//Вставляем данный переход в множество гототовых сработать.
        }
    }
    return result;
}


map <string, int> potFire(des::Automation& model, map <string, int> mark, string e){
    
    map<string, int> new_mark= mark;						//В переменную new_mark записываем значение данной маркировки.
    set<string> result = getPotReadyEvents(model, mark);	//В множество result записываем все переходы, готовые сработать.
    
    if (!result.empty() && result.find(e) == result.end()) {				//Если множество result не пустое и если данный переход содержится в множестве result
        throw invalid_argument("fire: name of firing event is invalid");	//Выводим предупреждение об ошибочности запущенного перехода
    }
    if (result.empty()) {		//Если словарь result пуст
        return new_mark;		//Возвращаем переменную new_mark.
    }
    string firing_event = e;			//Записываем в переменную firing_event данный переход.
    if (firing_event.empty()) {			//Если переменная firing_event пустая
        firing_event = *result.begin();	//Записываем в firing_event ссылку на первый элемент множества готовых сработать переходов.
    }
    for (const auto& input : model.getEventInputs(firing_event)) {	//Для всех состояний, которые ведут В запущенный переход.
        if (mark[input] == -1)		//Если маркировка состояния равна -1
            new_mark[input] = -1;	//Присваиваем переменной new_mark с параметрами текущего состояния значение -1.
        else						//Иначе
            new_mark[input] = mark[input] - model.getLinksFromStateToEvent(input, e);	//Присваиваем переменной new_mark с параметрами текущего состояния разность между маркировкой текущего состояния и количеством ссылок ИЗ текущего состояния В данный переход.

    }
    for (const auto& output : model.getEventOutputs(firing_event)) {	//Для всех состояний, которые ведут ИЗ запущенного перехода.
        if (mark[output] == -1)		//Если маркировка состояния равна -1
            new_mark[output] = -1;	//Присваиваем переменной new_mark с параметрами текущего состояния значение -1.
        else						//Иначе
            new_mark[output] = new_mark[output] + model.getLinksFromEventToState(e, output);	//Присваиваем переменной new_mark с параметрами текущего состояния сумму маркировки текущего состояния и количеством ссылок ИЗ данного перехода В текущее состояние.
    }
    return new_mark;	//Возвращаем переменную new_mark.
}

//Функция сравнения
bool compare(map<string, int> f, map<string, int> s) {
    for (const auto& pair : f) {			//Для маркировки f
        if (pair.second < s[pair.first]) {	//Если число токенов активности первой маркировки меньшу, чем число токенов активности второй
            return 0;						//Возвращаем ложь
        }
    }
    return 1;	//Иначе возвращаем правду
}

set <string> union_set(set <string> a, set <string> b ){
    for (auto el : b)
        a.insert(el);
    return a;
}

//Конструктор, создающий новый узел по родителю и маркировке
Node::Node(Node* par, map<string, int> d) {
    parent = par;			//Присваиваем полю parent значение переданного параметра par
    Node* p = par;			//Создаём еще один указатель на объект класса Node, равный par (p)
    map <string, int> less;	//объявляем переменную less для записи маркировки, которая меньше маркировки d
    while (p) {				//Пока указатель p не является пустым, выполняем следующие действия
        if (compare(d, p->data)) {	//если маркировка, вершины, хранящейся по адресу p, меньше, чем маркировка d,
            less = p->data;			//записываем эту маркировку в переменную less,
            break;					//прерываем цикл.
        }
        p = p->parent;				//Иначе переходим на одну вершину вверх по дереву (p = p -> parent).
    }
    if (!less.empty())					//Если маркировка less не пустая,
        for (const auto& pair : d) {	//для каждой позиции,
            if (pair.second > less[pair.first]) {	//в которой количество меток в маркировке less меньше, чем количество меток в маркировке d,
                d[pair.first] = -1;					//ставим количество меток в маркировке d равным -1.
            }
        }
    data = d;	//Присваиваем полю data значение маркировки d
}


//Метод обработки одного узла дерева
void Analyser::analyse_node(Node* start, des::Automation& model) {
    map<string, int> next;		//Создаем словарь next, в котором будет храниться имя состояния и количество токенов активности.
    const auto& ready_events = getPotReadyEvents(model, start->data);	//В словарь ready_events записываем все возможные события для данного узла модели
    
    if (ready_events.empty()) {	//Если пуст словарь ready_events,
       	term++;					//увеличиваем счетчик терминальных вершин term дерева.
    }
    else {	//Иначе
        for (const auto& elem : ready_events) {			//Для каждого элемента в словаре ready_events
            next = potFire(model, start->data, elem);	//записываем в словарь next маркировку.
            done_events.insert(elem);					//В контейнер done_events записываем текущий элемент elem, который является отработанным переходом, из списка полученных ready_events
            Node next_node(start, next);				//Создаем следующий узел

            if (close.find(next_node.data) == close.end() && start->data != next_node.data) {	//Если вершина с параметрами узла next_node уже содержится в контейнере закрытых вершин, и не равна начальной вершине
                open.insert(next_node.data);	//В множество открытых вершин записываем следующую вершину с параметрами next_node.
                int sizetr = tree.size();		//В целочисленную переменную sizetr записываем текущий размер дерева.
                tree[sizetr]=next_node;			//Записываем в дерево следующую вершину с параметрами next_node
            }
            else {	//Иначе
                if(tree[0].data == next_node.data)	//если параметры следующего узла равны параметрам начальной вершины дерева
                  dubl_start++;						//увеличиваем счетчик дублирующих начальную вершин
            }
        }
    }
}


//Проверка сети Петри на связность

int Analyser::bfs(des::Automation& model){
    set <string> visited;
    vector <string> queue;
    set <string> states = model.getStateNameSet();
    set <string> events = model.getEventNameSet();
    set <string> points = union_set(states, events);
    string first = *points.begin();
    string current;
    visited.insert(first);
    set <string> neighbours;

    queue.push_back(first);

    while(queue.size()!=0){
        current = queue[0]; //получаем первую в очереди вершину
        queue.erase(queue.begin());  //удаляем ее из очереди

        if (states.find(current)!=states.end())  //Если вершина - состояние
            neighbours = union_set(model.getStateInputs(current), model.getStateOutputs(current));
        else  //Если вершина - переход
            neighbours = union_set(model.getEventInputs(current), model.getEventOutputs(current));

        for (const auto n : neighbours){
            if (visited.find(n)==visited.end()){ //Если вершину еще не посетили
                visited.insert(n);
                queue.push_back(n);
            }
        }
    }
    if (visited != points){
        return 0;
        }
    else  {
        return 1;
    }
}

//Функция анализа сети Петри
map<string, bool> Analyser::run_analyse(des::Automation& model) {

    int i=0;	//счетчик, по умолчанию равный 0
    map<string, bool> analysis_result {{"alive", 0},{"coherent", 0},{"safe", 0},{"reachable",0}};	//Словарь, который и будет возвращать данная функция анализа сети Петри, и в котором содержатся пары ключ-значение, соответствующие характеристикам сети Петри
    map<string, int> mark;	//Словарь mark, в котором будет храниться маркировка в виде ключ-значение
    set <string> all_events = model.getEventNameSet();	//Контейнер, в котором хранятся все возможные события (переходы) для данной сети Петри

    for (const auto& s : model.getStateNameSet()) {	//Для всех состояний сети Петри
        mark[s] = model.getActivity(s);				//записываем в mark количество токенов активности
    }
    Node start(mark);	//Создаем начальную маркировку start как объект класса Node.

    tree[0] = start;	//В начальную вершину дерева tree записываем начальную маркировку.
    open.insert(start.data);	//Заносим начальную маркировку в множество открытых вершин.

    while (open.size()!=0) {	//Пока размер контейнера открытых вершин не равен 0
            analyse_node(&(tree[i]), model);	//Анализируем i-ую вершину дерева в методе analyse_node.
            close.insert((tree[i]).data);		//Заносим i-ую вершину дерева в множество закрытых вершин дерева.
            open.erase((tree[i]).data);			//Из множества открытых вершин стираем i-ую вершину дерева.
            i++;								//Увеличиваем счетчик i 
        }
    
    //Проверка на живость   
    if (term == 0 && (done_events.size() == all_events.size()))	//Если счетчик терминальных вершин равен 0 и размеры контейнеров done_events и all_events, содержащих соответственно все отработанные переходы и вообще все переходи сети Петри, равны между собой
        analysis_result["alive"]=1;							//В словаре analysis_result меняем значение ключа alive на 1
	
	//Проверка на достижимость
    if (dubl_start != 0)				//Если счетчик вершин, дублирующих начальную, равен нулю
        analysis_result["reachable"]=1;	//В словаре analysis_result меняем значение ключа reachable на 1

    //Проверка на безопасность
    analysis_result["safe"] = 1;
    for (const auto& el : close){
        for (const auto& pair: el){
            if (pair.second == -1)
                analysis_result["safe"] = 0;
        }
    }
    
    //Проверка на ограниченность
	analysis_result["coherent"] = bfs(model);
	
    open.clear();	//Очищаем множество открытых вершин
    close.clear();	//Очищаем множество закрытых вершин.
    term = 0;		//Обнуляем счетчик терминальных вершин.
    tree.clear();	//Очищаем дерево.
    dubl_start = 0;	//Обнуляем счетчик вершин, дублирующих начальную.
    done_events.clear();	//Очищаем множество выполненных переходов.

    return analysis_result;	//Возвращаем словарь analysis_result, в котором теперь записаны актуальные свойства данной сети Петри.
    }