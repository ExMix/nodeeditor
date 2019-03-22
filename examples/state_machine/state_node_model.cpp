#include "state_node_model.h"
#include "nodes/internal/Connection.hpp"

#include <QToolButton>
#include <QVBoxLayout>

class StateData : public NodeData
{
public:
    NodeDataType type() const override
    {
        return StateNodeModel::getTranstitionType();
    }
};

QString StateNodeModel::caption() const
{
    return QStringLiteral("State");
}

QString StateNodeModel::name() const
{
    return QStringLiteral("state_node");
}

unsigned int StateNodeModel::nPorts(PortType portType) const
{
    if (portType == PortType::In)
    {
        return input_data.size();
    }
    else
    {
        return output_data.size();
    }
}

NodeDataType StateNodeModel::dataType(PortType portType, PortIndex portIndex) const
{
    std::ignore = portType;
    std::ignore = portIndex;
    return getTranstitionType();
}

StateNodeModel::StateNodeModel()
{
    input_data.emplace_back(std::weak_ptr<NodeData>());
    output_data.emplace_back(new StateData());
}

NodeDataType StateNodeModel::getTranstitionType()
{
    return NodeDataType{"transition_port", ""};
}

void StateNodeModel::updatePorts()
{
    size_t i = 0;
    while (i < input_data.size() - 1)
    {
        if (input_data[i].lock() == nullptr)
        {
            input_data.erase(std::next(input_data.begin(), i));
            portRemoved(PortType::In, i);
        }
        else
        {
            ++i;
        }
    }

    if (input_data.back().lock() != nullptr)
    {
        input_data.push_back(std::weak_ptr<NodeData>());
        portAdded(PortType::In, input_data.size() - 1);
    }
}

QtNodes::NodeDataModel::ConnectionPolicy StateNodeModel::portOutConnectionPolicy(PortIndex) const
{
    return ConnectionPolicy::One;
}

QtNodes::NodeDataModel::ConnectionPolicy StateNodeModel::portInConnectionPolicy(PortIndex) const
{
    return ConnectionPolicy::One;
}

void StateNodeModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex port)
{
    input_data[port] = nodeData;
}

std::shared_ptr<NodeData> StateNodeModel::outData(PortIndex port)
{
    return output_data[port];
}

QWidget* StateNodeModel::embeddedWidget()
{
    QWidget* container = new QWidget();
    QVBoxLayout* vLayout = new QVBoxLayout(container);

    {
        QToolButton* button = new QToolButton();
        button->setText("Add");
        QObject::connect(button, &QToolButton::clicked, this, [this]()
        {
            output_data.emplace_back(new StateData());
            portAdded(PortType::Out, output_data.size());
        });
        vLayout->addWidget(button);
    }

    {
        QToolButton* button = new QToolButton();
        button->setText("Remove");
        QObject::connect(button, &QToolButton::clicked, this, [this]()
        {
            output_data.erase(output_data.begin());
            portRemoved(PortType::Out, 0);
        });
        vLayout->addWidget(button);
    }

    return container;
}
