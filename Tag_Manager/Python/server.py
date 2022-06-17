import socket
import sqlite3
import json
import os.path
import sys

HOST = '0.0.0.0'
# 目前端口是写死的
PORT = 10086
DATABASE_DIR = "../Database/"
# 这个函数貌似是之前测试用的，没什么大用
def readDataFromDatabase(request:str):
    print(request)
    conn = sqlite3.connect('../Database/testSqlite.db')
    c = conn.cursor()
    paras = [(1,),(2,)]
    selectRes = c.execute("SELECT * from testTable where id = ?", (int(request[3:]),))
    res = selectRes.fetchall();
    return res[0][1]

def getAllTagsByPlantName(req: dict):
    res = {}
    res['errCode'] = 0
    db = DATABASE_DIR + req[u'projName'] + '.db'
    plantName = req[u'plantName'];
    conn = sqlite3.connect(db)
    c = conn.cursor()
    stringRawRes = c.execute("SELECT stringTag.key, stringTag.value from plant, stringtag where stringtag.plantid = plant.id and plant.name = \'" + plantName + "\'")
    stringRes = stringRawRes.fetchall()
    stringTags = [{"key" : "__useless__", "value" : "__useless__"}]
    for oneTag in stringRes:
        stringTags.append({"key": oneTag[0], "value": oneTag[1]})
    res['stringTags'] = stringTags

    numberRawRes = c.execute("SELECT numberTag.key, numberTag.value from plant, numbertag where numbertag.plantid = plant.id and plant.name = \'" + plantName + "\'")
    numberRes = numberRawRes.fetchall()
    numberTags = [{"key" : "__useless__", "value" : -1}]
    for oneTag in numberRes:
        numberTags.append({"key": oneTag[0], "value": oneTag[1]})
    res['numberTags'] = numberTags
    return res

def getOneTagByPlantName(req: dict):
    res = {}
    res['errCode'] = 0
    db = DATABASE_DIR + req[u'projName'] + '.db'
    plantName = req[u'plantName']
    tagName = req[u'tag']
    if req[u'type'] == "string":
        conn = sqlite3.connect(db)
        c = conn.cursor()
        stringRawRes = c.execute("SELECT stringTag.key, stringTag.value from plant, stringtag where stringtag.plantid = plant.id and plant.name = \'" + plantName + "\' and stringtag.key = \'" + tagName + "\'")
        stringRes = stringRawRes.fetchall()
        stringTags = [{"key" : "__useless__", "value" : "__useless__"}]
        for oneTag in stringRes:
            stringTags.append({"key": oneTag[0], "value": oneTag[1]})
        res['stringTags'] = stringTags
        res['numberTags'] = [{"key" : "__useless__", "value" : -1}]
    elif req[u'type'] == "number":
        conn = sqlite3.connect(db)
        c = conn.cursor()
        numberRawRes = c.execute("SELECT numberTag.key, numberTag.value from plant, numbertag where numbertag.plantid = plant.id and plant.name = \'" + plantName + "\' and numbertag.key = \'" + tagName + "\'" )
        numberRes = numberRawRes.fetchall()
        numberTags = [{"key" : "__useless__", "value" : -1}]
        for oneTag in numberRes:
            numberTags.append({"key": oneTag[0], "value": oneTag[1]})
        res['numberTags'] = numberTags
        res['stringTags'] = [{"key" : "__useless__", "value" : "__useless__"}]
    else:
        res['errCode'] = -4
    return res

def findAllPlantsWithSomeTags(req: dict):
    res = {}
    res['errCode'] = 0
    db = DATABASE_DIR + req[u'projName'] + '.db'
    numberTags = req[u'numberTags']
    stringTags = req[u'stringTags']
    sql = ""
    note = ""
    for tag in numberTags:
        sql =sql + note + 'select distinct plant.name, plant.hierarchy from plant, numbertag where (plant.id = numbertag.plantid and numbertag.key = \'%s\' and numbertag.value >= %f and numbertag.value < %f) ' %(tag[u'key'], tag[u'lowerBound'], tag[u'upperBound'])
        note = " INTERSECT "
    for tag in stringTags:
        sql = sql + note +  'select distinct plant.name, plant.hierarchy from plant, stringtag where (plant.id = stringtag.plantid and stringtag.key = \'%s\' and stringtag.value = \'%s\') ' %(tag[u'key'], tag[u'value'])
        note = " INTERSECT "
    #     note = 'and'
    # sql = "select distinct plant.name, plant.hierarchy from plant, numbertag, stringtag " 
    # note = "where"
    # for tag in  numberTags:
    #     sql = sql + note +  ' (plant.id = numbertag.plantid and numbertag.key = \'%s\' and numbertag.value >= %f and numbertag.value < %f) ' %(tag[u'key'], tag[u'lowerBound'], tag[u'upperBound'])
    #     note = 'and'
    # for tag in stringTags:
    #     sql = sql + note +  ' (plant.id = stringtag.plantid and stringtag.key = \'%s\' and stringtag.value = \'%s\') ' %(tag[u'key'], tag[u'value'])
    conn = sqlite3.connect(db)
    c = conn.cursor()
    print(sql)
    selectRes = c.execute(sql)
    dbRes = selectRes.fetchall()
    print(dbRes)
    plants = [{'name' : '__useless__', 'hierarchy': -1}]
    for onePlant in dbRes:
        plants.append({'name': onePlant[0], 'hierarchy':onePlant[1]})
    res['plants'] = plants
    return res

def getAllPlantsInDatabase(req: dict):
    res = {}
    res['errCode'] = 0
    db = DATABASE_DIR + req[u'projName'] + '.db'
    conn = sqlite3.connect(db)
    c = conn.cursor()
    selectRes = c.execute("SELECT * from PLANT")
    dbRes = selectRes.fetchall()
    print(dbRes)
    plants = [{'name' : '__useless__', 'hierarchy': -1}]
    for onePlant in dbRes:
        plants.append({'name': onePlant[1], 'hierarchy':onePlant[2]})
    res['plants'] = plants
    return res

def getAllPlantsInOneHierarchy(req: dict):
    res = {}
    res['errCode'] = 0
    db = DATABASE_DIR + req[u'projName'] + '.db'
    hierarchy = req[u'hierarchy']
    conn = sqlite3.connect(db)
    c = conn.cursor()
    selectRes = c.execute("SELECT * from PLANT WHERE HIERARCHY = " + str(hierarchy))
    dbRes = selectRes.fetchall()
    print(dbRes)
    plants = [{'name' : '__useless__', 'hierarchy': -1}]
    for onePlant in dbRes:
        plants.append({'name': onePlant[1], 'hierarchy':onePlant[2]})
    res['plants'] = plants
    return res

# 读取数据库文件的统一接口，相应的函数对应DBApi.h接口中的函数
def getData(request: str):
    req = json.loads(request)
    print(req)
    method = req[u"method"]
    projName = req[u'projName']
    res = {}
    res['errCode'] = -1
    if not os.path.isfile(DATABASE_DIR + req[u'projName'] + '.db'):
        res['errCode'] = -2
        return json.dumps(res)
    
    if method == "getAllTagsByPlantName":
        res = getAllTagsByPlantName(req)
    elif method == "getOneTagByPlantName":
        res = getOneTagByPlantName(req)
    elif method == "findAllPlantsWithSomeTags":
        res = findAllPlantsWithSomeTags(req)
    elif method == "getAllPlantsInDatabase":
        res = getAllPlantsInDatabase(req)
    elif method == "getAllPlantsInOneHierarchy":
        res = getAllPlantsInOneHierarchy(req)
    print("result:")
    print(json.dumps(res))
    return json.dumps(res)

def echo_server():
    """Echo Server 的 Server 端"""

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # 对象s绑定到指定的主机和端口
    s.bind((HOST, PORT))
    # 只接受一个链接
    s.listen(1)

    while True:
        # accept表示接受用户端的连接
        conn, addr = s.accept()
        # 输出客户端地址
        print(f'Connect by {addr}')
        while True:
            data = conn.recv(1024 * 1024)
            print(data)
            if not data:
                break
            conn.sendall(bytes(getData(bytes.decode(data)),encoding="utf-8"))
        conn.close()
    s.close()


if __name__ == '__main__':
    DATABASE_DIR = sys.argv[1]
    print(DATABASE_DIR)
    echo_server()
