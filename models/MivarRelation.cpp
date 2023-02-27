#include "../services/SingleJSEngine.h"
#include "MivarRelation.h"

MivarRelation::RelationParameter::RelationParameter(const QString& name, const QString& type) {
    this->name = name;
    this->type = type;
}

MivarRelation::MivarRelation(const QString& id, const QString& name, const QString& description, const QString& code) {
    m_id = id;
    m_name = name;
    m_description = description;
    m_code = code;
}

const QString& MivarRelation::code() const noexcept {
    return m_code;
}

void MivarRelation::setCode(const QString& code) {
    m_code = code;
    sendEvent();
}

bool MivarRelation::addInput(const RelationParameter& param) {
    if (!containsParam(param.name)) {
        m_inputs.push_back(param);
        sendEvent();
        return true;
    }
    return false;
}

bool MivarRelation::addOutput(const RelationParameter& param) {
    if (!containsParam(param.name)) {
        m_outputs.push_back(param);
        sendEvent();
        return true;
    }
    return false;
}

const std::vector<MivarRelation::RelationParameter>& MivarRelation::outputs() const noexcept {
    return m_outputs;
}

const std::vector<MivarRelation::RelationParameter>& MivarRelation::inputs() const noexcept {
    return m_inputs;
}

void MivarRelation::removeParam(const QString& paramName) {
    for(size_t idx = 0; idx < m_inputs.size(); idx++)
        if (m_inputs[idx].name == paramName) {
            m_inputs.erase(m_inputs.begin() + idx);
            sendEvent();
            return;
        }
    for(size_t idx = 0; idx < m_outputs.size(); idx++)
        if (m_outputs[idx].name == paramName) {
            m_outputs.erase(m_outputs.begin() + idx);
            sendEvent();
            return;
        }
}

bool MivarRelation::containsParam(const QString& name) const noexcept {
    for(size_t idx = 0; idx < m_inputs.size(); idx++)
        if (m_inputs[idx].name == name)
            return true;
    for(size_t idx = 0; idx < m_outputs.size(); idx++)
        if (m_outputs[idx].name == name)
            return true;
    return false;
}

void MivarRelation::renameParam(const QString& paramName, const QString& newParamName) {
    if (!containsParam(newParamName)) {
        for(size_t idx = 0; idx < m_inputs.size(); idx++)
            if (m_inputs[idx].name == paramName) {
                m_inputs[idx].name = newParamName;
                sendEvent();
                return;
            }
        for(size_t idx = 0; idx < m_outputs.size(); idx++)
            if (m_outputs[idx].name == paramName) {
                m_outputs[idx].name = newParamName;
                sendEvent();
                return;
            }
    }
}

void MivarRelation::setParamType(const QString& paramName, const QString& type) {
    for(size_t idx = 0; idx < m_inputs.size(); idx++)
        if (m_inputs[idx].name == paramName) {
            m_inputs[idx].type = type;
            sendEvent();
            return;
        }
    for(size_t idx = 0; idx < m_outputs.size(); idx++)
        if (m_outputs[idx].name == paramName) {
            m_outputs[idx].type = type;
            sendEvent();
            return;
        }
}

QString MivarFunctionRelation::toJSFunction() const {
    QString code = "(function(";
    if (m_inputs.size()) {
        code += m_inputs[0].name;
        for (size_t i = 1; i < m_inputs.size(); i++)
            code += ", " + m_inputs[i].name;
    }
    code += ") {";
    if (m_outputs.size()) {
        code += "var " + m_outputs[0].name;
        for (size_t i = 1; i < m_outputs.size(); i++)
            code += ", " + m_outputs[i].name;
        code += ";\n";
    }
    code += m_code;
    if (m_outputs.size() == 1) {
        code += "\n;return " + m_outputs[0].name + ";";
    } else if (m_outputs.size() > 1) {
        
    }
    code += "})";
    return code;
}

MivarRelation::ErrorData MivarFunctionRelation::errorDetails() const {
    QJSValue func = SingleJSEngine::engine().evaluate(toJSFunction());
    if (func.isError())
        return { func.property("lineNumber").toInt() - 1, func.toString() };
    return { -1, "" };
}

const QString MivarFunctionRelation::type() const noexcept {
    return "prog";
}
