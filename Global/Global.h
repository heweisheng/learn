#pragma once
typedef struct 
{
	int x;
}Data;
class Global
{
public:
	Global();
	~Global();
	void SetData(Data *User=NULL);
	Data GetDate();
private:
	Data *point;
};
