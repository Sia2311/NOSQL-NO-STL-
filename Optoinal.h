#pragma once

template <typename T>
class Optional{
    private:
    bool exsists; //есть ли значение
    T data;

    public:
    Optional(){
        exsists = false;
        data = T();
    }
    //с значением
    Optional(const T& value){
        exsists = true;
        data = value;
    }

    bool HAS_value() const{
        return exsists;
    }
    T value() const {
        return data;
    }

};
