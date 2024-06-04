#ifndef Engine_Function_h__
#define Engine_Function_h__
#include <fstream>
//#include <string>
#include <tuple>
using namespace std;
namespace Engine
{
	// ���ø��� ����� ������������ �ڷ����� ���������� ���� ��
	// ����� �ν��Ͻ�ȭ �ϱ� ���Ͽ� �����δ� Ʋ

	template<typename T>
	void	Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void	Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete[] Pointer;
			Pointer = nullptr;
		}
	}


	template<typename T>
	unsigned int Safe_AddRef(T& pInstance)
	{
		unsigned int		iRefCnt = { 0 };

		if (nullptr != pInstance)
			iRefCnt = pInstance->AddRef();

		return iRefCnt;
	}

	template<typename T>
	unsigned int Safe_Release(T& pInstance)
	{
		unsigned int		iRefCnt = { 0 };

		if (nullptr != pInstance)
		{
			iRefCnt = pInstance->Release();

			if (0 == iRefCnt)
				pInstance = nullptr;
		}

		return iRefCnt;
	}

	template<typename T>
	void Save_Data_Extraction(std::ofstream& outFile, const T& data)
	{
		//���� ���ڿ��� ���� ��� 
		if constexpr (is_same_v<T, wstring>)
		{
			//���̸� ���� ���� �ϰ� �� ���� ���ڸ� ����
			size_t size = data.size();
			outFile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
			outFile.write(reinterpret_cast<const char*>(data.c_str()), size * sizeof(wchar_t));
			
		}
		else
		{	//���ڿ��� �� ������ ���
			outFile.write(reinterpret_cast<const char*>(&data), sizeof(T));
		}
		//������ �׽�Ʈ�� �ڵ� 
		//int tmp = 0;
		//tmp = sizeof(T);


	}

	template<typename T1, typename... Types>
	void Save_Data(T1 FilePath, Types... arg)
	{
		std::wstring fullPath = L"../../Data/" + std::wstring(FilePath) + L".bin";

		
		std::ofstream outFile(fullPath, ios::out | ios::binary/*| std::ios::app*/);

		if (outFile.is_open())
		{
			//Save_Data(FilePath, 1,2,3,4,5,6) �̷��� ȣ�� �ƴٸ� 
			((Save_Data_Extraction(outFile, arg)), ...);	//���� ǥ����
			//Save_Data_Extraction�� 
			//Save_Data_Extraction(outFile, 1);
			//Save_Data_Extraction(outFile, 2);
			//Save_Data_Extraction(outFile, 3);
			//Save_Data_Extraction(outFile, 4);
			//Save_Data_Extraction(outFile, 5);
			//Save_Data_Extraction(outFile, 6);
			//�̷��� 6��ȣ��ȴ�.

			//�̷��� �ص� ��
			//((outFile.write(reinterpret_cast<const char*>(&arg), sizeof(arg))), ...);
		}


		outFile.close();
	}

	template<typename T1, typename... Types>
	void Save_Data_Text(T1 FilePath, Types... arg)
	{
		std::wstring fullPath = L"../../Data/" + std::wstring(FilePath) + L".txt";


		std::wofstream outFile(fullPath);
		if (outFile.is_open())
		{
			((outFile << arg << std::endl), ...);
		}


		outFile.close();
	}




	template<typename T>
	void Load_Data_Extraction(std::ifstream& inFile, T& data)
	{
		if constexpr (is_same_v<T, wstring>)	//���� Ÿ���� wstring�̶�� 
		{
			size_t size = 0;
			inFile.read(reinterpret_cast<char*>(&size), sizeof(size_t));		//����� �о����
			wchar_t* buffer = new wchar_t[size + 1];		//null���� ����ؼ� +1 ��ŭ �Ҵ�
			ZeroMemory(buffer, sizeof(wchar_t) * (size + 1));
			inFile.read(reinterpret_cast<char*>(buffer), size * sizeof(wchar_t));	//���ڿ��� �о����
			data = buffer;		//���ڿ��� tuple�� �־��ֱ�
			delete[] buffer;
		}
		else
		inFile.read(reinterpret_cast<char*>(&data), sizeof(T));
	}

	// ���Ͽ��� �����͸� �о���� �Լ� (����� ȣ��)
	template<_uint iArgCnt = 0, typename... Types>
	void Load_Data_Recursive(ifstream& inFile, tuple<Types...>& pdata)
	{
		
		//������ Ÿ�ӿ� Ż�� ������ �� ������
		// Load_Data()�� ȣ�� �� �� ��������� Ÿ�԰� ������ �ְ� 
		//

		const int tmp = sizeof...(Types);	//���� Ÿ���� ����   // sizeof...(arg): �Ķ���� ���� ������ ��ȯ�Ѵ�.
		if constexpr (iArgCnt < tmp)							//���ǽ��� ���� ���� �����Ͻð��� �ڵ带 ������ c++17���� �����ϴ� ���
		{														//��� Ż�� ������ �����Ͻð��� üũ�Ѵ�
																//�׷��Ƿ� ����� �񱳴���� �� �� �ִ�.
			Load_Data_Extraction(inFile, get<iArgCnt>(pdata));	//Ʃ���� iArgCnt��°�� ���� ���� ä���
			Load_Data_Recursive<iArgCnt + 1>(inFile, pdata);	//��������� ȣ���Ѵ�.
		}
	}

	// ���Ͽ��� �����͸� �о���� �Լ� (Ÿ�� ����) ��������� � Ÿ������ ��������� ��
	template<typename... Types>	//Ÿ���� ������ �Ǿ��ٸ�, 
	//Ʃ���� ��ȯ���� ������ Ÿ������ ��ȯ��Ų��
	tuple<Types...> Load_Data(const wstring& FilePath)
	{
		wstring fullPath = L"../../Data/" + FilePath + L".bin";
		ifstream inFile(fullPath, ios::in | ios::binary);
		//Ʃ���� �����.
		tuple<Types...> data;

		if (inFile.is_open())
		{
			//Ʃ�ÿ� ���� ä���.(Load_Data_Recursive �� �Լ� ������ ��������� ä��� ���� )
			//Load_Data_Recursive()�� �Լ��� ȣ���� �� ������ Load_Data_Recursive<?,?>�̷������� ȣ���ؾ� ������
			//Load_Data()�� ȣ���� �� ��������� Ÿ���� ��������� ������
			//�����Ϸ��� �˾Ƽ� Ÿ���� �߷��ϰ�, �Ϲ������� Load_Data_Recursive�Լ��� �μ��� �־��ش�.
			Load_Data_Recursive(inFile, data);
		}

		inFile.close();

		return data;
	}


	template<typename T>
	void Load_Data_Extraction_Text(std::wifstream& inFile, T& data)
	{
		std::wstring line;
		if (std::getline(inFile, line))
		{
			std::wstringstream ss(line);
			ss >> data;
		}
	}

	template<std::size_t iArgCnt = 0, typename... Types>
	void Load_Data_Recursive_Text(std::wifstream& inFile, std::tuple<Types...>& pdata)
	{
		constexpr std::size_t tmp = sizeof...(Types);
		if constexpr (iArgCnt < tmp)
		{
			Load_Data_Extraction_Text(inFile, std::get<iArgCnt>(pdata));
			Load_Data_Recursive_Text<iArgCnt + 1>(inFile, pdata);
		}
	}





	template<typename... Types>
	std::tuple<Types...> Load_Data_Text(const std::wstring& FilePath)
	{
		std::wstring fullPath = L"../../Data/" + FilePath + L".txt";
		std::wifstream inFile(fullPath);
		std::tuple<Types...> data;

		if (inFile.is_open())
		{
			Load_Data_Recursive_Text(inFile, data);
		}

		inFile.close();

		return data;
	}











	template<typename T>
	T RandomNum(T RandomRangeX,T RandomRangeY)
	{
		random_device randomDevice;
		mt19937_64 Gen(randomDevice());

		uniform_real_distribution<T> RandomDist(RandomRangeX, RandomRangeY);
		T RandomValue = RandomDist(Gen);

		return RandomValue;



	}



	//�̹� ���ǵǾ� �ִٰ�?
	//int RandSign()
	//{
	//	random_device randomDevice;
	//	mt19937_64 Gen(randomDevice());
	//
	//	uniform_real_distribution<float> RandomDist(0.f, 1.f);
	//	int iRandomSign = RandomDist(Gen) > 0.5f ? 1 : -1;
	//
	//
	//
	//	return iRandomSign;
	//}



}

#endif // Engine_Function_h__
