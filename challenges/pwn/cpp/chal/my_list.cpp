#include <stdint.h>
#include <iostream>
#include <memory>
#include <cstdio>
#include <stdint.h>

__attribute__((constructor)) void ignore_me() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

struct List {
    virtual char* pop() = 0;
    virtual void push(const char* data) = 0;
};

struct Node {
    std::string data;
    std::unique_ptr<Node> next;
};

struct MyList : public List {
    MyList() : head{nullptr} {};

    void push(const char* data) {
        head = std::unique_ptr<Node>(
                new Node{std::string(data), std::move(head)});
    }

    char* pop() {
        if(head == nullptr) {
            return nullptr;
        }

        char * data = head->data.data();
        head = std::move(head->next);

        return data;
    }

    void clean() {
        while(head) {
            head = std::move(head->next);
        }
    }

    ~MyList() {
        clean();
    }

    friend std::ostream& operator<<(std::ostream &os, const MyList &list);

private:
    std::unique_ptr<Node> head;
};

std::ostream& operator<<(std::ostream &os, const MyList &list) {
    Node *head = list.head.get();
    while(head) {
        os << head->data << ' ';
        head = head->next.get();
    }
    return os;
}

void list_menu(MyList* li)
{
    while (true) {
        char buf[0x10] = {0};
        std::string inp;
        std::cout << "Enter your choice: " << std::endl;
        fgets(buf, 0x60, stdin);

        switch(atoi(buf)) {
            case 1:
                std::cout << "Enter the data you want to save: ";
                std::cin >> inp;
                li->push(inp.c_str());

                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                break;
            case 2: { 
                char* ret = li->pop();
                if (ret == nullptr) break;
                std::cout << "Data: " << ret << std::endl;
                break;
            }
            case 3:
                std::cout << *li << std::endl;
                break;
            default:
                std::cout << "1 = push to list" << std::endl;
                std::cout << "2 = pop from list" << std::endl;
                std::cout << "3 = print list" << std::endl;
        }
    }
}

int main(void)
{
    MyList li;
    list_menu(&li);
}
