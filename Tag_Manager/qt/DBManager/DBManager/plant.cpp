#include "plant.h"
#include "QFileDialog"
#include <string>

plant::plant()
    :name("__default__")
{

}

inline void copyFromBuffer(unsigned char *buffer, unsigned int& idx, void * destination, size_t size){
    memcpy(destination, (void*)(buffer + idx), size);
    idx += size;
}

plant::plant(QString filename){
    QFile importFile(filename);
    if (!importFile.open(QIODevice::ReadOnly | QIODevice::Truncate))
    {
        return;
    }
    auto byteArray = importFile.readAll();
    this->name = "__default__";
    unsigned char* buffer =(unsigned char*) byteArray.data();
    unsigned int idx = 0;
    size_t length;
    copyFromBuffer(buffer, idx, (void*)(&length),sizeof(length));
    int hierarchy;
    copyFromBuffer(buffer, idx, (void*)(&hierarchy), sizeof(int));
    this->hierarchy = hierarchy;
    while(idx < length){
        char type;
        copyFromBuffer(buffer, idx, (void*)(&type), sizeof(char));
        tag tag;
        if(type == 0x1){
            tag.stringTag = true;
            size_t keySize;
            copyFromBuffer(buffer, idx, (void*)(&keySize), sizeof(keySize));
            unsigned char* keyBuf = new unsigned char[keySize];
            copyFromBuffer(buffer, idx, (void*)(keyBuf), keySize);
            tag.key = QString::fromStdString(std::string((char*)keyBuf, keySize));
            delete[] keyBuf;
            size_t valueSize;
            copyFromBuffer(buffer, idx, (void*)(&valueSize), sizeof(valueSize));
            unsigned char *valueBuf = new unsigned char[valueSize];
            copyFromBuffer(buffer, idx, (void*)(valueBuf), valueSize);
            tag.stringValue = QString::fromStdString(std::string((char*)valueBuf, valueSize));
            delete[] valueBuf;
        }
        else{
            tag.stringTag = false;
            size_t keySize;
            copyFromBuffer(buffer, idx, (void*)(&keySize), sizeof(keySize));
            unsigned char* keyBuf = new unsigned char[keySize];
            copyFromBuffer(buffer, idx, (void*)(keyBuf), keySize);
            tag.key = QString::fromStdString(std::string((char*)keyBuf, keySize));
            delete[] keyBuf;
            float value;
            copyFromBuffer(buffer, idx, (void*)(&value), sizeof(value));
            tag.numberValue = value;
        }
        this->tags.append(tag);
    }

}


inline void copyToBuffer(unsigned char * buffer, unsigned int& idx, void *source, size_t size){
    memcpy((void*)(buffer + idx), source, size);
    idx += size;
}

bool plant::exportPlant(QString file){
    size_t length = sizeof(size_t);
    length += sizeof(int);
    for(auto& tag : this->tags){
        //string tag: 0x1 + keySize + key + valueSize + value
        if(tag.stringTag){
            length += sizeof(unsigned char) + 2 * sizeof(size_t) + tag.key.length() + tag.stringValue.length();
        }
        //number tag: 0x0 + keySize + key + value
        else{
            length += sizeof(unsigned char) + sizeof(size_t) + tag.key.length() + sizeof(tag.numberValue);
        }
    }
    unsigned char* buffer = new unsigned char[length];
    unsigned int idx = 0;
    copyToBuffer(buffer, idx, (void*)(&length), sizeof(size_t));
    auto hierarchy = this->hierarchy;
    copyToBuffer(buffer, idx, (void*)(&hierarchy), sizeof(hierarchy));
    for(auto& tag : this->tags){
        if(tag.stringTag){
            unsigned char type = 0x1;
            copyToBuffer(buffer, idx, (void*)(&type), sizeof(unsigned char));
            auto stdKey = tag.key.toStdString();
            size_t size = stdKey.size();
            copyToBuffer(buffer, idx, (void*)(&size), sizeof(size));
            copyToBuffer(buffer, idx, (void*)(stdKey.c_str()),stdKey.size());
            auto stdValue = tag.stringValue.toStdString();
            size = stdValue.size();
            copyToBuffer(buffer, idx, (void*)(&size), sizeof(size));
            copyToBuffer(buffer,idx,(void*)(stdValue.c_str()), stdValue.size());
        }
        else{
            unsigned char type = 0x0;
            copyToBuffer(buffer, idx, (void*)(&type), sizeof(unsigned char));
            auto stdKey = tag.key.toStdString();
            size_t size = stdKey.size();
            copyToBuffer(buffer, idx, (void*)(&size), sizeof(size));
            copyToBuffer(buffer, idx, (void*)(stdKey.c_str()),stdKey.size());
            float value = tag.numberValue;
            copyToBuffer(buffer, idx, (void*)(&value), sizeof(value));
        }
    }
    QFile exportFile(file);
    if (!exportFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        delete[] buffer;
        return false;
    }
    exportFile.write((char*)buffer, length);
    delete[] buffer;
    return true;
}
