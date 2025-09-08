#include <iostream>
#include<string>
#include<fstream>

#include<vector>
using namespace std;


struct Result {
    int auth_count=0;
    int avatars_loaded_count=0;
    bool success ;
};

Result Parsing(std::string path) 
{
    Result result;
    int auth_count = 0;
    std::string line;
    std::ifstream logfile(path);
    if(logfile.is_open()){
    
    std::vector<std::string> Logs;
    while(std::getline(logfile, line )) {
        if (line.find("Пользователь авторизован , создание JWT токена") != std::string::npos||line.find(("Роль Пользователя"))!= std::string::npos) {
            result.auth_count++;
        }
        if(line.find("ссылка на аву")!= std::string::npos) {
            result.avatars_loaded_count++;
        }
        result.success=true;
        Logs.push_back(line);
        
    }
    logfile.close();
    return result;
    }
    else {
        cout<<"Не удалось открыть файл по данному пути , попробуйте ещё раз"<<endl;
        result.success=false;
    }

    return result;

}
int main()
{
    std::string path;
    cout<<"Добро пожаловать в txt Парсер для GameRank \n Введи путь до файла с логом:"<<endl;
    cin>>path;
    if(!path.empty())
    {
        Result res = Parsing(std::string (path));
        if (res.success==true) {
        int auth_count= res.auth_count;
        int avatars_loaded_count=res.avatars_loaded_count;
        cout<<"Статистка по Сервису Авторизации :"<<endl;
        cout<<"Авторизовалось пользователей : " <<auth_count<<endl;
        cout<<"Загружено Аватаров:" <<avatars_loaded_count<<endl;
        cout<<"Парсер ещё в разработке , идите"<<endl;
        }
        
    }
    else{
        cout<<"Путь к файлу недействителен , попробуйте ещё раз"<<endl;
    }

}