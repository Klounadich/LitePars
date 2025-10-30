
#include <cstddef>
#include <iostream>
#include<fstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
using namespace std;
const string LogsDirectoryPath="/home/klounadich/dev/other/logs";
const string SolutionArchivePath = "/home/klounadich/dev/other/Archive.txt";


struct ScanData {
    bool success =false;
    map<int , string> logsfiles;
};

struct ParseData {
    vector<string> data;
    int count = 0;

    

};

string SearchSolution(string active_error) {
    string line;
    ifstream file(SolutionArchivePath);
    
    if (file.is_open()) {  
        while (getline(file, line)) {
            size_t point = line.find("|");
            if(point != string::npos) {
                string error_text = line.substr(0, point);
                string solution = line.substr(point + 1);  
                solution.erase(0, solution.find_first_not_of(" \t"));
                solution.erase(solution.find_last_not_of(" \t") + 1);

                if(active_error.find(error_text) != string::npos) {
                    return solution;
                }
            }
        }
        file.close();
    }
    
    return ""; 
}



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
        if (line.find("Successfully logged in")!= string::npos) { // лог при авторизации
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
        if (line.find("New user created ")!= string::npos) { // лог при реге
            result.count++;
        }
        }
    }
        
    file.close();
     return result;
}



ParseData WarningParse(string filepath) {
    ParseData result;
    ifstream file(filepath);

    string line;
    string currentError;
    bool collectingError = false;

    while (getline(file, line)) {
        
        bool isNewError = (line.find("warn:") != string::npos) ||
                         (line.find("fail:") != string::npos) ||
                         (line.find("error:") != string::npos);

        if (isNewError) {
            
            if (!currentError.empty()) {
                result.data.push_back(currentError);
            }
            
            currentError = line;
            collectingError = true;
        }
        else if (collectingError) {
            
            if (!line.empty()) {
                
                if (line[0] == ' ' || line[0] == '\t') {
                    currentError += "\n" + line;
                } else {
                    
                    result.data.push_back(currentError);
                    currentError.clear();
                    collectingError = false;
                }
            }
        }
    }

  
    if (!currentError.empty()) {
        result.data.push_back(currentError);
    }

    file.close();
    return result;
}
int main() {
    string skip;
    int filenumber;
    bool repeat = true;
    cout<<"Добро Пожаловать \n Для сканирования директории введите любой символ ..."<<endl;
    cin>>skip;
    cout<<"Начинаем Сканирование"<<endl;
    while (repeat) {
    
    
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
                        cout<<"=========================================================================================="<<endl;
                        cout<<result.data.at(i) <<endl;
                        string check = SearchSolution(result.data.at(i));
                        if(!check.empty()) {
                            cout<<"---------------------------------------------------"<<endl;
                            cout<<"Ваша ошибка найдена в списке уже решённых проблем"<<endl;
                            cout<<"Увидеть предложенное решение ? (y/n)"<<endl;
                            char choice;
                            cin >>choice;
                            if( choice== 'y'|| choice =='Y') {
                                cout<<"||ОТВЕТ ИЗ АРХИВА: "<<check<<" ||"<<endl;
                            }
                        
                        }
                        if(i+1 < result.data.size()) {
                        cout<<"Найдено ещё " << result.data.size() -i-1<< "\n Ввведите любой символ для вывода следующей ошибки"<< endl;
                        cin>> skip;
                        }
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
     char choice;
        cout << "\nХотите выполнить еще один анализ? (y/n): ";
        cin >> choice;
        
        if(choice != 'y' && choice != 'Y') {
            repeat = false;
            cout << "Выход из программы. До свидания!" << endl;
        }
        cout << endl; 
    }

    return 0;
}