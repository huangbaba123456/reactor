#ifndef  EVENTHANDLER_H_
#define EVENTHANDLER_H_

typedef int Handle;

class EventHandler{
public:
    virtual ~EventHandler(){};
    virtual Handle get_handle() const=0;
    virtual void handle_read()=0;
    virtual void handle_write()=0;
    virtual void handle_error()=0;
};




#endif // ! E