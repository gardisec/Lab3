#include "lab3.h"


int main(int argc, char *argv[]) {
    try {
        request request = getRequest(argc, argv);
        if (argc == 1){
            cout << "0 arguments were entered." << endl;
            return 1;
        }
        if (request.query.get_size() == 0){
            cout << "query is empty" << endl;
            return 1;
        }
        if (!isFileExist(request.file) || request.file.empty()){
            throw runtime_error(("Error: wrong file name or file doesn't exist"));
        }

        structures strucType = structRequest(request.query[0]);
        request.query[0] = delFirstLetter(request.query[0]);

        switch (strucType) {
            case SET:
                request.query[0] = delFirstLetter(request.query[0]); // -e
                request.query[0] = delFirstLetter(request.query[0]); // -t
                switch (commandRequest(request.query[0])) {
                    case add:
                        setAdd(request);
                        break;
                    case del:
                        setDel(request);
                        break;
                    case at:
                        setAt(request);
                        break;
                    case print:
                        printSet(request);
                        break;
                    default:
                        cout << "Unknown Set command" << endl;
                }
                break;
            case ARRAY:
                switch (commandRequest(request.query[0])) {
                    case push:
                        arrPush(request);
                        break;
                    case del:
                        arrDel(request);
                        break;
                    case Get:
                        arrGet(request);
                        break;
                    case print:
                        printArr(request);
                        break;
                    default:
                        cout << "Unknown Array command" << endl;
                }
                break;
            case STACK:
                switch (commandRequest(request.query[0])) {
                    case push:
                        stackPush(request);
                        break;
                    case pop:
                        stackPop(request);
                        break;
                    case Get:
                        stackGet(request);
                        break;
                    case print:
                        printStack(request);
                        break;
                    default:
                        cout << "Unknown Stack command" << endl;
                }
                break;
            case LIST:
                switch (commandRequest(request.query[0])) {
                    case push:
                        listPush(request);
                        break;
                    case del:
                        listDel(request);
                        break;
                    case Get:
                        listGet(request);
                        break;
                    case print:
                        printList(request);
                        break;
                    default:
                        cout << "Unknown List command" << endl;
                }
                break;
            case QUEUE:
                switch (commandRequest(request.query[0])) {
                    case push:
                        queuePush(request);
                        break;
                    case pop:
                        queuePop(request);
                        break;
                    case Get:
                        queueGet(request);
                        break;
                    case print:
                        printQueue(request);
                        break;
                    default:
                        cout << "Unknown Queue command" << endl;
                }
                break;
            case HASHMAP:
                switch (commandRequest(request.query[0])) {
                    case insert:
                        hashSetInsert(request);
                        break;
                    case del:
                        hashSetDel(request);
                        break;
                    case Get:
                        hashSetGet(request);
                        break;
                    case print:
                        printHash(request);
                        break;
                    default:
                        cout << "Unknown HashMap command" << endl;
                }
                break;
            case TREE:
                switch (commandRequest(request.query[0])) {
                    case insert:
                        treeInsert(request);
                        break;
                    case Search:
                        treeSearch(request);
                        break;
                    case complete:
                        treeComplete(request);
                        break;
                    case print:
                        printTree(request);
                        break;
                    default:
                        cout << "Unknown Tree command" << endl;
                }
                break;
            default:
                cout << "Unknown structure type" << endl;
        }
    }
    catch (exception &ex) {
        cout << ex.what() << endl;
        return -1;
    }
}

