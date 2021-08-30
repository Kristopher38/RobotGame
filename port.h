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

    IPort(Block* owner) : owner(owner) {}
	virtual std::string GetTypeStr() = 0;
	virtual PortType GetType() = 0;
    virtual void Connect(IPort* port) { connections.insert(port); }
};

class InputPort : public IPort
{
public:
    InputPort(Block* owner) : IPort(owner) {}
    virtual std::string GetTypeStr() { return "Input"; }
    virtual PortType GetType() { return PortType::INPUT; }
};

class OutputPort : public IPort
{
public:
    OutputPort(Block* owner) : IPort(owner) {}
    virtual std::string GetTypeStr() { return "Output"; }
    virtual PortType GetType() { return PortType::OUTPUT; }
};

#endif // PORT_H
