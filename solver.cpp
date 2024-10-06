#include<bits/stdc++.h>
#define endl '\n'
#define pii pair<int,int>
#define vvi vector<vector<int>>
#define pino pair<int, Node*>
#define f first
#define s second

// Felipe Lages de Lima

using namespace std;

vector<vector<int>> inicial(5, vector<int>(5, 0)), final(5, vector<int>(5));
vector<vvi> visitados;

struct Node
{
    Node *pai;
    int h;
    vvi estado;
    int level;

    Node() : pai(NULL), h(0) {}
    Node(vvi matriz, Node* anterior) : pai(anterior), estado(matriz) {}
    Node(int hr, vvi matriz, Node* anterior, int lvl) : h(hr), pai(anterior), estado(matriz), level(lvl) {}
};

void setar();
int heur(vvi matriz);
vvi troca(vvi atual, int i, int j, int x, int y);
bool visitei(vvi matriz);
pii procura(vvi matriz, int target);
void mostra(vvi matriz);
void mostra_caminho(Node *atual);
void bfs();
void dfs();
void busca_gulosa();
void best_first();

int main()
{
    setar();

    best_first();
    //bfs();
    //dfs();
    //busca_gulosa();

    return 0;
}

void setar()
{   
    for(int i = 1; i <= 3; i++)
    {
        for(int j = 1; j <= 3; j++)
        {
            cin >> inicial[i][j];
            final[i][j] = j + 3 * (i - 1);
        }
    }

    cout << endl;
    final[3][3] = 0;
}

int heur(vvi matriz)
{
    int h = 0;

    for(int i = 1; i <= 3; i++)
    {
        for(int j = 1; j <= 3; j++)
        {
            if(matriz[i][j] != final[i][j])
            {
                pii diff = procura(final, matriz[i][j]);
                h += abs(i - diff.f) + abs(j - diff.s);
            }
        }
    }

    return h;
}

pii procura(vvi matriz, int target)
{
    for(int i = 1; i <= 3; i++)
    {
        for(int j = 1; j <= 3; j++)
        {
            if(matriz[i][j] == target) return {i, j};
        }
    }

    return {};
}

vvi troca(vvi matriz, int i, int j, int x, int y)
{
    int temp = matriz[i][j];
    matriz[i][j] = matriz[x][y];
    matriz[x][y] = temp;

    return matriz;
}

bool visitei(vvi matriz)
{
    for(auto estado : visitados)
    {
        if(estado == matriz) return true;
    }

    return false;
}

void mostra_estado(vvi matriz)
{
    for(int i = 1; i <= 3; i++)
    {
        for(int j = 1; j <= 3; j++) cout << matriz[i][j] << " ";
        cout << endl;
    }

    cout << endl;
}

void mostra_caminho(Node *atual)
{
    int passo = 1;

    vector<vvi> caminho;

    while(atual != NULL)
    {
        caminho.push_back(atual->estado);
        atual = atual->pai;
    }
    
    for(int i = caminho.size() - 1; i >= 0; i--) 
    {
        cout << "Passo " << passo << endl;
        mostra_estado(caminho[i]);
        passo++;
    }
}

vector<vvi> moves(vvi matriz)
{
    vector<vvi> swaps;

    pii pos = procura(matriz, 0);

    swaps.push_back(troca(matriz, pos.f, pos.s, pos.f - 1, pos.s));
    swaps.push_back(troca(matriz, pos.f, pos.s, pos.f + 1, pos.s));
    swaps.push_back(troca(matriz, pos.f, pos.s, pos.f, pos.s - 1));
    swaps.push_back(troca(matriz, pos.f, pos.s, pos.f, pos.s + 1));

    return swaps;
}

void best_first()
{
    priority_queue<pino, vector<pino>, greater<pino>> fila;
    Node *origem = new Node(heur(inicial), inicial, NULL, 0);

    fila.push({origem->h, origem});

    while(!fila.empty())
    {
        Node *atual = fila.top().s;
        fila.pop();

        if(visitei(atual->estado)) continue;
        visitados.push_back(atual->estado);

        if(atual->h == 0)
        {
            mostra_caminho(atual);
            cout << "FIM DO CAMINHO" << endl;
            break;
        }

        vector<vvi> swaps = moves(atual->estado);

        for(auto matriz : swaps)
        {
            if(!visitei(matriz))
            {
                Node *novo = new Node(heur(matriz), matriz, atual, atual->level + 1);

                fila.push({novo->h + novo->level, novo});
            }
        }
    }
}

void bfs()
{
    queue<Node*> fila;

    Node* origem = new Node(inicial, NULL);

    fila.push(origem);

    while(!fila.empty())
    {
        Node* atual = fila.front();

        fila.pop();
        
        if(visitei(atual->estado)) continue;
        visitados.push_back(atual->estado);

        if(atual->estado == final)
        {
            mostra_caminho(atual);
            cout << "FIM DO CAMINHO" << endl;
            break;
        }

        vector<vvi> swaps = moves(atual->estado);

        for(auto matriz : swaps)
        {
            if(!visitei(matriz))
            {
                Node *novo = new Node(matriz, atual);

                fila.push(novo);
            }
        }
    }
}

void dfs()
{
    stack<Node*> pilha;

    Node* origem = new Node(inicial, NULL);

    pilha.push(origem);

    while(!pilha.empty())
    {
        Node* atual = pilha.top();

        pilha.pop();
        
        if(visitei(atual->estado)) continue;
        visitados.push_back(atual->estado);

        if(atual->estado == final)
        {
            mostra_caminho(atual);
            cout << "FIM DO CAMINHO" << endl;
            break;
        }

        vector<vvi> swaps = moves(atual->estado);

        for(auto matriz : swaps)
        {
            if(!visitei(matriz))
            {
                Node *novo = new Node(matriz, atual);

                pilha.push(novo);
            }
        }
    }
}

void busca_gulosa()
{
    priority_queue<pino, vector<pino>, greater<pino>> fila;
    Node *origem = new Node(heur(inicial), inicial, NULL, 0);

    fila.push({origem->h, origem});

    while(!fila.empty())
    {
        Node *atual = fila.top().s;

        fila.pop();

        if(visitei(atual->estado)) continue;
        visitados.push_back(atual->estado);

        if(atual->h == 0)
        {
            mostra_caminho(atual);
            cout << "FIM DO CAMINHO" << endl;
            break;
        }

        vector<vvi> swaps = moves(atual->estado);

        for(auto matriz : swaps)
        {
            if(!visitei(matriz))
            {
                Node *novo = new Node(heur(matriz), matriz, atual, atual->level + 1);

                fila.push({novo->h, novo});
            }
        }
    }
}
