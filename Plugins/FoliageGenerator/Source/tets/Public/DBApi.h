/*
	  quu..__
	   $$$b  `---.__
		"$$b        `--.                          ___.---uuudP
		 `$$b           `.__.------.__     __.---'      $$$$"              .
		   "$b          -'            `-.-'            $$$"              .'|
			 ".                                       d$"             _.'  |
			   `.   /                              ..."             .'     |
				 `./                           ..::-'            _.'       |
				  /                         .:::-'            .-'         .'
				 :                          ::''\          _.'            |
				.' .-.             .-.           `.      .'               |
				: /'$$|           .@"$\           `.   .'              _.-'
			   .'|$u$$|          |$$,$$|           |  <            _.-'
			   | `:$$:'          :$$$$$:           `.  `.       .-'
			   :                  `"--'             |    `-.     \
			  :##.       ==             .###.       `.      `.    `\
			  |##:                      :###:        |        >     >
			  |#'     `..'`..'          `###'        x:      /     /
			   \                                   xXX|     /    ./
				\                                xXXX'|    /   ./
				/`-.                                  `.  /   /
			   :    `-  ...........,                   | /  .'
			   |         ``:::::::'       .            |<    `.
			   |             ```          |           x| \ `.:``.
			   |                         .'    /'   xXX|  `:`M`M':.
			   |    |                    ;    /:' xXXX'|  -'MMMMM:'
			   `.  .'                   :    /:'       |-'MMMM.-'
				|  |                   .'   /'        .'MMM.-'
				`'`'                   :  ,'          |MMM<
				  |                     `'            |tbap\
				   \                                  :MM.-'
					\                 |              .''
					 \.               `.            /
					  /     .:::::::.. :           /
					 |     .:::::::::::`.         /
					 |   .:::------------\       /
					/   .''               >::'  /
					`',:                 :    .'
										 `:.:'

	  Ƥ��������д�����Ѿ��ܾ��ˣ�������������һ�ߣ���Ϣһ��
	  ��˵��ллƤ����
	  */
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "CoreMinimal.h"
#include <string>
#include <vector>
/**
 * The comments are all written in GB2312.
 * �������һ���ӿ��࣬Ŀǰ��ʹ��TCP�ķ�ʽ��Python�������л�ȡ��Դ������������Ż�������DBApi.cpp�н�����Ӧ�޸ġ�
 * ��Unreal Engine��������ж����ݿ�ķ��ʶ���ͨ����Щ�ӿڡ�
 * errno 0���������
 * errno -1��json�ļ��ķ���������
 * errno -2��û���ҵ���Ӧ����Ŀ���ݿ�
 * errno -3���������û�гɹ������ݿ�����
 */
class TETS_API DBApi
{
public:
	DBApi();
	~DBApi();
	//���Ӧ����Private������Public��������ʧ����^.^
	static std::string getDataFromDatabase(std::string request);
	struct stringKV {
		FString key;
		FString value;
	};

	struct numberKV {
		FString key;
		float value;
	};

	struct numberKVRequest {
		FString key;
		float lowerBound;
		float upperBound;
	};

	enum tagType {
		NUMBER,
		STRING
	};
	struct plant {
		FString name;
		int hierarchy;
	};

	//����һ��ֲ�����ƣ�Ѱ����ȫ��tag
	USTRUCT()
		struct getAllTagsByPlantNameRequest {
		FString projName;
		FString plantName;

	};
	USTRUCT()
		struct getAllTagsByPlantNameResult {
		int errCode;
		TArray<stringKV> stringTags;
		TArray<numberKV> numberTags;
	};
	static getAllTagsByPlantNameResult getAllTagsByPlantName(getAllTagsByPlantNameRequest request);

	//����һ��ֲ�����ƺ�tag�����࣬Ѱ���ڸ��������µ�����tag
	USTRUCT()
		struct getOneTagByPlantNameRequest {
		FString projName;
		FString plantName;
		tagType type;
		FString tag;
	};
	USTRUCT()
		struct getOneTagByPlantNameResult {
		int errCode;
		TArray<stringKV> stringTags;
		TArray<numberKV> numberTags;
	};
	static getOneTagByPlantNameResult getOneTagByPlantName(getOneTagByPlantNameRequest request);

	//Ѱ��ӵ��һЩ��ǩ��ֲ�ӵ����Щ��ǩ�е�����һ�����������Ҫ��
	//�����ַ��ͱ�ǩ��Ѱ��key��value��Ҫƥ�䣬��ֵ�ͱ�ǩ��keyƥ�䲢��value��Ҫ����һ����Χ
	USTRUCT()
		struct findAllPlantsWithSomeTagsRequest {
		FString projName;
		TArray<stringKV> stringTags;
		TArray<numberKVRequest> numberTags;
	};
	USTRUCT()
		struct findAllPlantsWithSomeTagsResult {
		int errCode;
		TArray<plant> plants;
	};
	static findAllPlantsWithSomeTagsResult findAllPlantsWithSomeTags(findAllPlantsWithSomeTagsRequest request);

	//��ȡdatabase�����е�ֲ��
	USTRUCT()
		struct getAllPlantsInDatabaseRequest {
		FString projName;
	};
	USTRUCT()
		struct getAllPlantsInDatabaseResult {
		int errCode;
		TArray<plant> plants;
	};
	static getAllPlantsInDatabaseResult getAllPlantsInDatabase(getAllPlantsInDatabaseRequest request);

	//��ȡĳ���㼶�����е�ֲ��
	USTRUCT()
		struct getAllPlantsInOneHierarchyRequest {
		FString projName;
		int hierarchy;
	};
	USTRUCT()
		struct getAllPlantsInOneHierarchyResult {
		int errCode;
		TArray<plant> plants;
	};
	static getAllPlantsInOneHierarchyResult getAllPlantsInOneHierarchy(getAllPlantsInOneHierarchyRequest request);
};
