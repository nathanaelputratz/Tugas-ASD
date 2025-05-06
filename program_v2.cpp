#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <cmath>

using namespace std;

// Struct untuk membuat node untuk expression tree
struct Node {
    string value; // menyimpan tipe data char
    Node* left;
    Node* right;
};

// Fungsi untuk memeriksa apakah sebuah character merupakan operator
bool checkOperator(string &c) {
    // Jika merupakan operator maka akan mengembalikan nilai True
    return c == "+" || c == "-" || c == "*" || c == "/" || c == "^";
}


// Fungsi untuk menentukan prioritas dari operator (+, -, *, /)
int precedence(string &op) {
    if (op == "+" || op == "-") {
        // Jika + atau - maka akan mengembalikan nilai 1
        return 1;
    }

    if (op == "*" || op == "/"){
        // Jika * atau / maka akan mengembalikan nilai 2
        return 2;
    }

    if (op == "^") {
        // Jika ^ maka akan mengembalikan nilai 3
        return 3;
    }
    // jika bukan dari ke-empat operator diatas maka 0
    return 0;
}

// Fungsi untuk mengubah ekspresi infix menjadi postfix
vector<string> infixToPostfix(string infix) {
    stack<string> operators;
    vector<string> postfix;
    string num;

    for (int i = 0; i < infix.length(); i++) {
        char curr = infix[i];

        if (isspace(curr)) continue; // Jika merupakan spasi maka continue
        if (isdigit(curr) || curr == '.') {
            // jika character merupakan angka atau titik maka concate
            // pada string num

            num += curr;
        } else {
            // jika num tidak empty (ada) maka push ke dalam vektor postfix
            // hapus num atau hilangkan seluruh karakter
            if (!num.empty()) {
                postfix.push_back(num);
                num.clear();
            }

            string op(1, curr); // casting char kedalam string

            if (curr == '(') {
                // Jika karakter adalah (, push ke stack
                operators.push(op);
            } else if (curr == ')') {
                // Jika karakter adalah tanda kurung tutup,
                // pop semua operator sampai menemukan (

                while (!operators.empty() && operators.top() != "(") {
                    postfix.push_back(operators.top());
                    operators.pop();
                }

                if (!operators.empty()) {
                    // Hapus ( dari stack
                    operators.pop();
                }
            } else if (checkOperator(op)) {
                // Jika karakter adalah operator, pop operator dari stack ke postfix
                // selama operator di stack memiliki prioritas lebih tinggi atau sama
                while (!operators.empty() && precedence(operators.top()) >= precedence(op)) {
                    postfix.push_back(operators.top());
                    operators.pop();
                }
                operators.push(op); // Push operator ke stack
            }
        }
    }

    // Jika masih ada angka tersisa di 'num', push ke postfix
    if (!num.empty()) {
        postfix.push_back(num);
    }
    
    // Pop semua operator yang tersisa di stack ke postfix
    while (!operators.empty()) {
        postfix.push_back(operators.top());
        operators.pop();
    }

    return postfix; // Kembalikan hasil postfix sebagai vector string
}

// Fungsi untuk membuat expression tree dari ekspresi postfix
Node* createExpressionTree(vector<string> &postfix) {
    stack<Node*> st;

    for (string token : postfix) {
        // Buat node dan assign value dengan token
        Node* node = new Node();
        node->value = token;

        if (!checkOperator(token)) {
            // jika bukan operator maka inisialisasi
            // left-child dan right-child sebagai NULL
            node->left = node->right = NULL;
        } else {
            // jika merupakan operator maka,
            // pop dua node dari stack sebagai child dan tambahkan
            // ke left child dan right child 
            node->right = st.top();
            st.pop();
            node->left = st.top();
            st.pop();
        }
        // push node kedalam stack
        st.push(node);
    }

    return st.top();
}

// Fungsi untuk melakukan graph transversal postorder
void getPostorder(Node* root, vector<string> &output) {
    if (!root) return;
    getPostorder(root->left, output); // pergi ke left child
    getPostorder(root->right, output); // pergi ke right child
    output.push_back(root->value); // push value kedalam output
}

// Fungsi untuk melakukan evaluasi expression tree
float evaluate(Node* root) {
    vector<string> postfix;
    getPostorder(root, postfix);
    stack<float> st;
    
    for (string token : postfix) {
        if (!checkOperator(token)) { 
            // jika bukan operator maka masukkan ke dalam stack
            st.push(stof(token)); // casting tipe data ke float
        } else {
            // jika merupakan operator maka
            // ambil dua nilai teratas dari stack
            float right = st.top(); 
            st.pop();
            float left = st.top();
            st.pop();
            if (token == "+") {
                // jika token merupakan + maka jumlahkan
                st.push(left + right);
            } else if (token == "-") {
                // jika token merupakan - maka kurangi
                st.push(left - right);
            } else if (token == "*") {
                // jika token merupakan * maka kalikan
                st.push(left * right);
            } else if (token == "/") {
                // jika token merupakan / maka bagi
                st.push(left / right);
            } else if (token == "^") {
                // jika token merupakan ^ maka pangkatkan
                st.push(pow(left, right));
            }
        }
    }
    // nilai akhir merupakan nilai stack paling atas (terakhir)
    return st.top();
}

int main() {
    string t;
    cout << "Massukan ekspresi infix yang hendak di evaluasi : ";
    getline(cin, t);
    vector<string> postfix = infixToPostfix(t);
    for (string s : postfix) {
        cout << s << " ";
    }
    cout << endl;
    Node* root = createExpressionTree(postfix);
    cout << "Hasil : " << evaluate(root) << "\n";

    return 0;
}