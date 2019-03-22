#pragma once

#include <nodes/NodeDataModel>

using QtNodes::NodeData;
using QtNodes::PortIndex;
using QtNodes::PortType;
using QtNodes::NodeDataType;
using QtNodes::Connection;

class StateNodeModel : public QtNodes::NodeDataModel
{
public:
    StateNodeModel();
    static NodeDataType getTranstitionType();

    void updatePorts() override;

    QString caption() const override;
    QString name() const override;

    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    ConnectionPolicy portOutConnectionPolicy(PortIndex) const override;
    ConnectionPolicy portInConnectionPolicy(PortIndex) const override;

    void setInData(std::shared_ptr<NodeData> nodeData, PortIndex port) override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    std::vector<std::weak_ptr<NodeData>> input_data;
    std::vector<std::shared_ptr<NodeData>> output_data;
};