// balda_old.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>


using namespace std;

int letterNum = 5;
//const int n = 5;  //число сторон (должно быть нечетным)
char board[5][5] = {
'-','-', '-', '-', '-',
'-','-', '-', 'а', 'ж',
'к','о', 'ф', 'т', 'а',
'с','т', 'е', 'л', 'л',
'и','-', 'я', 'ь', 'п' };
ifstream file;


void findWord(vector<char> str);
struct Node
{
    Node* up;
    Node* right;
    Node* down;
    Node* left;

    vector<pair<int, int>> pos; //запоминает в каких вершинах были его предки
    int x; //столбец
    int y; //строчка

    char data;

    bool flag;

    Node()
    {
        up = nullptr;
        right = nullptr;
        down = nullptr;
        left = nullptr;
        data = '-';
        x = 0;
        y = 0;
        flag = 0;
    }
    Node(Node* u, Node* r, Node* d, Node* l)
    {
        up = u;
        right = r;
        down = d;
        left = l;
        data = '-';
        x = 0;
        y = 0;
        flag = 0;

    }
    Node(char d)
    {
        up = nullptr;
        right = nullptr;
        down = nullptr;
        left = nullptr;
        data = d;
        if (data == '-')
            flag = 1;
        else
            flag = 0;
        x = 0;
        y = 0;
    }
    Node(const Node& obj) : up(obj.up), right(obj.right), down(obj.down), left(obj.left), data(obj.data)
    {
        for (int i = 0; i < obj.pos.size(); i++)
        {
            pos[i] = obj.pos[i];
        }
    }
    /*~Node()
    {
        delete ptr[1];
    }*/
};
struct Tree
{
    Node* head;

    Tree()
    {
        head = nullptr;
    }
    Tree(Node* h, int x_, int y_)
    {
        head = h;
    }
    void print()
    {
        cout << endl;
        cout << head->up->data << " ";
        cout << head->right->data << " ";
        cout << head->down->data << " ";
        cout << head->left->data << " ";
    }

};

void printVector(vector<char> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i];
    }
    cout << endl;
    return;
}
bool check(Node* current, pair<int, int> pos_)
{
    if (current->flag == 1 && board[pos_.second][pos_.first] == '-')
    {
        return 0;
    }
    for (auto i = 0; i < current->pos.size(); i++)
    {
        if (current->pos[i] == make_pair(pos_.first, pos_.second))
            return 0; //сосед плохой
    }
    return 1; //сосед хороший
}
void createNodes(Node* head) //создаёт 4 узла вокруг клетки
{
    int i = head->y;
    int j = head->x;
    if ((i > 0) && check(head, make_pair(j, i - 1)))
    {
        head->up = new Node(board[i - 1][j]);
        head->up->y = i - 1;
        head->up->x = j;
        head->up->pos = head->pos;
        head->up->pos.push_back(make_pair(j, i));
        if (head->flag)
            head->up->flag = head->flag;
    }
    if (j < 5 && check(head, make_pair(j + 1, i)))
    {
        head->right = new Node(board[i][j + 1]);
        head->right->y = i;
        head->right->x = j + 1;
        head->right->pos = head->pos;
        head->right->pos.push_back(make_pair(j, i));
        if (head->flag)
            head->right->flag = head->flag;

    }

    if (i < 5 && check(head, make_pair(j, i + 1)))
    {
        head->down = new Node(board[i + 1][j]);
        head->down->y = i + 1;
        head->down->x = j;
        head->down->pos = head->pos;
        head->down->pos.push_back(make_pair(j, i));
        if (head->flag)
            head->down->flag = head->flag;

    }
    if (j > 0 && check(head, make_pair(j - 1, i)))
    {
        head->left = new Node(board[i][j - 1]);
        head->left->y = i;
        head->left->x = j - 1;
        head->left->pos = head->pos;
        head->left->pos.push_back(make_pair(j, i));
        if (head->flag)
            head->left->flag = head->flag;
    }


    return;
}
void createTree(Tree& tree, Node* start, int depth, int currentDepth)
{
    if (currentDepth < depth)
    {
        createNodes(start);
        currentDepth++;
        if (start->up != nullptr)
            createTree(tree, start->up, depth, currentDepth);
        if (start->right != nullptr)
            createTree(tree, start->right, depth, currentDepth);
        if (start->down != nullptr)
            createTree(tree, start->down, depth, currentDepth);
        if (start->left != nullptr)
            createTree(tree, start->left, depth, currentDepth);
    }
    else return;
}
void printTree(Tree& tree, Node* start, int depth, vector<char> vec) //печатает ветки подходящих слов
{
    vec.push_back(start->data);
    if (start->up == nullptr && start->right == nullptr && start->down == nullptr && start->left == nullptr)
    {
        if (vec.size() == depth && start->flag == 1)
        {

            findWord(vec);
        }
        vec.pop_back();
        return;
    }
    if (start->up != nullptr)
    {
        printTree(tree, start->up, depth, vec);
    }

    if (start->right != nullptr)
    {

        printTree(tree, start->right, depth, vec);
    }

    if (start->down != nullptr)
    {

        printTree(tree, start->down, depth, vec);
    }

    if (start->left != nullptr)
    {
        printTree(tree, start->left, depth, vec);
    }
    return;
}


void findWord(vector<char> str)
{
    file.open("russian_nouns.txt", ios::in);
    int length = str.size();

    char* c = new char[50];

    int i = 0;

    while (file.getline(c, 50))
    {
        if (c[length] == '\0')
        {
            for (int j = 0; j < length; j++)
            {
                if (str[j] == '-')
                {
                    if (j == length - 1)
                    {
                        i = 0;
                        break;
                    }
                    else
                    {
                        i++; continue;
                    }
                }
                else if (str[j] == c[j])
                {
                    i++; continue;
                }
                else
                {
                    i = 0;
                    break;
                }
            }
            if (i == length)
            {
                //printVector(str);
                cout << c << "\n";
                i = 0;
            }
        }
    }
    file.close();
    return;
}
void printBoard()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    return;
}

int main()
{
    setlocale(LC_ALL, "");
    file.open("russian_nouns.txt", ios::in);
    printBoard();

    Tree tree;
    int depth = 6;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            Node start(board[i][j]);
            start.x = j;
            start.y = i;
            tree.head = &start;

            createTree(tree, &start, depth, 1);
            vector<char> buf;
            printTree(tree, &start, depth, buf);
        }
    }

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
