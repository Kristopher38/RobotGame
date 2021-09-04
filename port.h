#ifndef PORT_H
#define PORT_H

#include <string>
#include <vector>
#include <unordered_set>
#include <variant>

class Block;

enum class PortType {INPUT, OUTPUT};

typedef std::monostate nil;

typedef std::variant<nil, bool, int64_t, double, std::string> DataValue;
typedef std::variant<nil, bool, int64_t, double, std::string, std::vector<DataValue>> DataValueEx;

class IPort
{
public:
    std::unordered_set<IPort*> connections;
    Block* owner;
    DataValueEx data;
    DataValueEx nextData;

    IPort(Block* owner) : owner(owner), data(0), nextData(0) {}
    IPort(const IPort& other)
    {
        this->connections = other.connections;
        this->owner = other.owner;
        this->data = other.data;
        this->nextData = other.nextData;
    }
    virtual std::string GetTypeStr() = 0;
	virtual PortType GetType() = 0;
    virtual IPort* Clone() = 0;
    virtual bool IsInput() = 0;
    virtual bool IsOutput() = 0;

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
    virtual bool CanConnect(IPort* port = nullptr)
    {
        if (!port)
            return this->IsOutput() ||
                   this->IsInput() && this->connections.size() == 0;
        else
            return this->GetType() != port->GetType() &&
                   (this->IsInput() && this->connections.size() == 0 ||
                    port->IsInput() && port->connections.size() == 0);
    }
    virtual void Update(DataValueEx newData)
    {
        this->nextData = newData;
    }
    virtual DataValueEx GetData()
    {
        return this->data;
    }
    virtual void Swap()
    {
        this->data = this->nextData;
    }
    virtual void Propagate()
    {
        if (this->IsOutput())
            for (auto conn : this->connections)
                if (conn->IsInput())
                    conn->Update(this->nextData);
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
    virtual bool IsInput() { return true; }
    virtual bool IsOutput() { return false; }
};

class OutputPort : public IPort
{
public:
    OutputPort(Block* owner) : IPort(owner) {}
    OutputPort(const OutputPort& other) : IPort(other) {}
    virtual std::string GetTypeStr() { return "Output"; }
    virtual PortType GetType() { return PortType::OUTPUT; }
    virtual OutputPort* Clone() { return new OutputPort(*this); }
    virtual bool IsInput() { return false; }
    virtual bool IsOutput() { return true; }
};

#endif // PORT_H
