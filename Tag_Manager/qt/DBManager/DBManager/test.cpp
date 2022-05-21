#include "test.h"
#include "plant.h"
#include <iostream>
test::test()
{
}

void test::runPlantTest(){
    plant t;
    t.hierarchy = 6;
    plant::tag tag1;
    tag1.stringTag = true;
    tag1.key = "helloworld";
    tag1.stringValue = "yeshello";
    t.tags.append(tag1);

    plant::tag tag2;
    tag2.stringTag = false;
    tag2.key = "numberTest";
    tag2.numberValue = 60.314159;
    t.tags.append(tag2);

    plant::tag tag3;
    tag3.stringTag = true;
    tag3.key = "stringTest";
    tag3.stringValue = "content";
    t.tags.append(tag3);

    t.exportPlant("./plantTest.mod");

    plant o("./tree.mod");
    using namespace std;
    cout << "hierarchy: " << o.hierarchy << endl;
    cout << "==================" << endl;
    for(auto tag: o.tags){
        if(tag.stringTag){
            cout << "stringTag: "<< tag.key.toStdString() << endl;
            cout << "stringValue: " << tag.stringValue.toStdString() << endl;
            cout << "==================" << endl;
        }
        else{
            cout << "numberTag: "<< tag.key.toStdString() << endl;
            cout << "numberValue: " << tag.numberValue << endl;
            cout << "==================" << endl;
        }
    }
}
