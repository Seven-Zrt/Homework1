#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"parser.h"
char* strComponentType(Component* compPtr) {

    char* compTypeName = new char[6];
    switch (compPtr->getType()) {

    case VSource: strcpy(compTypeName, "V"); break;
    case Resistor: strcpy(compTypeName, "R"); break;
    case BJT: strcpy(compTypeName, "T"); break;
    case MOSFET: strcpy(compTypeName, "M"); break;
    case ISource: strcpy(compTypeName, "I"); break;
    case Inductor: strcpy(compTypeName, "ind"); break;
    case Diode: strcpy(compTypeName, "Diode"); break;
    case Capacitor: strcpy(compTypeName, "Cap"); break;
    }

    return compTypeName;
}
void printComponents(Component* compPtr) {
    char compTypeName[6];
    cout << endl << "Components: " << endl << endl;
    while (compPtr != NULL) {
        strcpy(compTypeName, strComponentType(compPtr));
        cout << "->" << compTypeName << compPtr->getcompNum();
        compPtr = compPtr->getNext();
    }
    cout << endl;
    return;
}
void printNodes(Node* nodePtr, int compFlag) {

    Connections* conPtr;
    cout << endl << "Nodes: " << endl << endl;
    while (nodePtr != NULL) {
        if (compFlag == 0) { //It is printed just the names of the nodes
            cout << "nodenumber: " << nodePtr->getNameNum();
        }
        else if (compFlag == 1) { //It is printed the nodes and the connections
            cout << "nodenumber: " << nodePtr->getNameNum() << "       " << "component amount: ";
            conPtr = nodePtr->getConList();
            cout << nodePtr->getCount() << endl;
            while (conPtr->next != NULL) {
                cout << "num:" << conPtr->comp->getcompNum() << "   value:" << conPtr->comp->getVal() << "     name : " << conPtr->comp->getName() << endl;
                conPtr = conPtr->next;
            }
            cout << "num:" << conPtr->comp->getcompNum() << "    value:" << conPtr->comp->getVal() << "     name:" << conPtr->comp->getName() << endl;
        }
        else {
            cout << "Invalid value for compFlag. (0) to print just nodes, (1) to print nodes and connections!";
            exit(1);

        }

        nodePtr = nodePtr->getNext();
    }


    return;
}
int main() {
    FILE* in = fopen("C:/Users/12520/Desktop/netlist.txt", "r");
    FILE* out = fopen("C:/Users/12520/Desktop/out.txt", "w");
    char buf[1024];
    CompHead comlist;
    NodeHead nodelist;
    Component* com1, * com2, * comptr;//用于遍历的节点
    Node* nodePtr, * nodePtr1, * nodePtr2;
    int intBuf1, intBuf2, intBuf3, intBuf4;
    while (fgets(buf, sizeof(buf), in) != NULL)
    {
        if (buf[0] == '.' || buf[0] == '*') {
            continue;
        }
        else {
            int i = 0;//计数
            char* str;
            int nonum1, nonum2;
            char comname[NameLength];
            double val;
            str = strtok(buf," ");//用空格截取字符串
            strcpy(comname, str);
            i++;
            Component* com;
            CompType typebuf;
            switch (str[0]) {
            case 'V':typebuf = VSource; break;
            case 'R':typebuf = Resistor; break;
            case 'C':typebuf = Capacitor; break;
            }
            while (str != NULL) {
                str = strtok(NULL, " ");//处理后续字符串
                i++;
                if (i == 2) nonum1 = atoi(str);
                else if (i == 3) nonum2 = atoi(str);
                else if(i==4) val = strtod(str,NULL);//转化节点号和权值
            }
            //初始化组件
            com = new Component(typebuf, val, NA, nonum1, nonum2, NA, NA, NULL, comname);
            comlist.addComp(com);
        }
    }
    com1 = comlist.getComp(0);//节点连接组件
    while (com1 != NULL) {
        for (int b = 0; b < 3; b++) { 
            if ((!com1->isCon(b)) && (com1->getConVal(b) != NA)) { 
                intBuf1 = com1->getConVal(b); 
                nodePtr1 = nodelist.addNode();
                nodePtr1->setNameNum(intBuf1); 
                com1->connect(b, nodePtr1); 
                nodePtr1->connect(b, com1);

               
                com2 = com1->getNext();
                while (com2 != NULL) {
                    for (int c = 0; c < 3; c++) { 
                        if (com2->getConVal(c) == intBuf1) { 
                            com2->connect(c, nodePtr1);
                            nodePtr1->connect(c, com2);
                            break;                                  
                        }
                    }
                    com2 = com2->getNext();
                }
            }
        }
        com1 = com1->getNext();
    }
    int datum = 0;//默认参考节点为0
    nodePtr = nodelist.getNode(0); //找lastnode
    int lastnode = nodePtr->getNameNum();
    while (nodePtr != NULL) {
        lastnode = (nodePtr->getNameNum() > lastnode) ? nodePtr->getNameNum() : lastnode;
        nodePtr = nodePtr->getNext();
    }
    printf("Title:*test for tran\n");
    printf("datum=%d       lastnode=%d\n", datum, lastnode);
    nodePtr = nodelist.getNode(0);
    printNodes(nodePtr, 1);
    fclose(in);
    fclose(out);
	return 0;
}