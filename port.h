#ifndef PORT_H
#define PORT_H

#include <string>
#include <vector>
#include <unordered_set>

class Block;

enum class PortType {INPUT, OUTPUT};

typedef int DataValue;

class IPort
{
public:
    std::unordered_set<IPort*> connections;
    Block* owner;
	DataValue data;
    DataValue nextData;

    IPort(Block* owner) : owner(owner) {}
    IPort(const IPort& other)
    {
        this->connections = other.connections;
        this->owner = other.owner;
        this->data = other.data;
    }
    virtual std::string GetTypeStr() = 0;
	virtual PortType GetType() = 0;
    virtual IPort* Clone() = 0;
    virtual void Connect(IPort* port)
    {
        port->connections.insert(this);
        connections.insert(port);
    }
    virtual void Disconnect(IPort* port)
    {
        connections.erase(port);
        port->connections.erase(this);
    }
    virtual void Update(DataValue newData)
    {
        this->nextData = newData;
    }
    virtual void Swap()
    {
        this->data = this->nextData;
    }
    virtual ~IPort()
    {
        for (auto conn = connections.begin(); conn != connections.end(); ++conn)
            (*conn)->connections.erase(this);
    }
};

class InputPort : public IPort
{
public:
    InputPort(Block* owner) : IPort(owner) {}
    InputPort(const InputPort& other) : IPort(other) {}
    virtual std::string GetTypeStr() { return "Input"; }
    virtual PortType GetType() { return PortType::INPUT; }
    virtual InputPort* Clone() { return new InputPort(*this); }
};

class OutputPort : public IPort
{
public:
    OutputPort(Block* owner) : IPort(owner) {}
    OutputPort(const OutputPort& other) : IPort(other) {}
    virtual std::string GetTypeStr() { return "Output"; }
    virtual PortType GetType() { return PortType::OUTPUT; }
    virtual OutputPort* Clone() { return new OutputPort(*this); }
};

#endif // PORT_H
