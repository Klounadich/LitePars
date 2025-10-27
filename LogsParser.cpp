#include <algorithm>
#include <iostream>
#include<fstream>
#include <string>
#include <type_traits>
#include <vector>
#include <map>
#include <filesystem>
using namespace std;
const string LogsDirectoryPath="/home/klounadich/dev/other/logs";

struct ScanData {
    bool success =false;
    map<int , string> logsfiles;
};

struct ParseData {
    vector<string> data;
    int count = 0;

    

};
ScanData Scan() {
    ScanData result;
    int counter = 0;
    for( auto& file : filesystem::recursive_directory_iterator(LogsDirectoryPath)) {
        result.logsfiles[counter ]= file.path().string();
        counter++;
    }
    result.success=true;
    return result;

}

ParseData AuthCountParse(string filepath) {
    ParseData result;
    string line;
    
    ifstream file ( filepath);
    if (file.is_open()) {
        while (getline(file , line)) {
        if (line == "Успешная Авторизация") { // лог при авторизации
            result.count++;
        }
        }
    }
    file.close();
    return result;
}

ParseData RegCountParse(string filepath) {
     ParseData result;
     string line;
    
    ifstream file ( filepath);
    if (file.is_open()) {
        while (getline(file , line)) {
        if (line == "Успешная Регистрация") { // лог при реге
            result.count++;
        }
        }
    }
    file.close();
     return result;
}

ParseData WarningParse(string filepath) {
     ParseData result;
     string line;
     ifstream file (filepath);
     if (file.is_open()) {
        while (getline(file , line)) {
        if (line.find("Warn")!= string::npos || line.find("Error")!= string::npos ) { // искать содержит ли варн или  error
            result.data.push_back(line);
        }
        }
    }
     return result;
}

int main() {
    string skip;
    int filenumber;
    cout<<"Добро Пожаловать \n Для сканирования директории введите любой символ ..."<<endl;
    cin>>skip;
    cout<<"Начинаем Сканирование"<<endl;
    ScanData check = Scan();
    if(check.success == true) {
        cout<<"Найдены следующие файлы :" <<endl;
        for(int i =0 ; i<check.logsfiles.size(); i++) {
            cout<< i << "|| "<<check.logsfiles.at(i)<<endl;
        }
        cout<<"Для продолжения укажите номер файла для прочтения"<<endl;
        cin>> filenumber;
        
            int num ;
            cout<<"Выберите тип отбираемых данных : \n  1.Кол-во авторизовавшихся пользователей (Auth-Microservice) \n 2.Кол-во зарегистрировавшихся пользователей (Auth-Microservice) \n 3.Предупреждения/Ошибки"<<endl;
            cin>> num;
            ParseData result;
            switch (num) {
                case 1:
                result= AuthCountParse(check.logsfiles.at(filenumber));
                if (result.count !=0) {
                    cout<< "Кол-во авторизовавшихся пользователей указанных в логе  : " << result.count <<endl;
                }
                else {
                    cout<< "Не удалось найти ни одного авторизовавшегося пользователя :(" <<endl;
                }
                break;
                case 2:
                result= RegCountParse(check.logsfiles.at(filenumber));
                if (result.count !=0) {
                    cout<< "Кол-во зарегестрированных пользователей указанных в логе  : " << result.count <<endl;
                }
                else {
                    cout<< "Не удалось найти ни одного зарегистрированного пользователя :(" <<endl;
                }
                break;
                case 3:
                result = WarningParse(check.logsfiles.at(filenumber));
                if( !result.data.empty()) {
                    for(int i=0 ; i< result.data.size() ; i++) {
                        cout<<result.data.at(i) <<endl;
                    }
                }
                else {
                    cout<<" Не найденного ни одного предупреждения или ошибки"<<endl;
                }
                break;


            }
            
            
        
    }
    else {
        cout<<"Не удалось просканировать папку . Проверьте её " << LogsDirectoryPath<<endl;
    }
    return 0;
}