#include "MivarRelation.h"

MivarRelation::MivarRelation(const QString& id, const QString& name, const QString& description) {
    m_id = id;
    m_name = name;
    m_description = description;
}

const QString& MivarRelation::code() const noexcept {
    return m_code;
}

void MivarRelation::setCode(const QString& code) {
    m_code = code;
}

bool MivarRelation::addInput(const RelationParameter& param) {
    if (!containsParam(param.name())) {
        m_inputs.push_back(param);
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
        if (m_inputs[idx].name() == paramName) {
            m_inputs.erase(m_inputs.begin() + idx);
            return;
        }
    for(size_t idx = 0; idx < m_outputs.size(); idx++)
        if (m_outputs[idx].name() == paramName) {
            m_outputs.erase(m_outputs.begin() + idx);
            return;
        }
}

bool MivarRelation::addOutput(const RelationParameter& param) {
    if (!containsParam(param.name())) {
        m_outputs.push_back(param);
        return true;
    }
    return false;
}

bool MivarRelation::containsParam(const QString& name) const noexcept {
    for(size_t idx = 0; idx < m_inputs.size(); idx++)
        if (m_inputs[idx].name() == name)
            return true;
    for(size_t idx = 0; idx < m_outputs.size(); idx++)
        if (m_outputs[idx].name() == name)
            return true;
    return false;
}


QString MivarFunctionRelation::toJSFunction() const {
    QString code = "(function(";
    if (m_inputs.size()) {
        code += m_inputs[0].name();
        for (size_t i = 1; i < m_inputs.size(); i++)
            code += ", " + m_inputs[i].name();
    }
    code += ") {";
    if (m_outputs.size()) {
        code += "var " + m_outputs[0].name();
        for (size_t i = 1; i < m_outputs.size(); i++)
            code += ", " + m_outputs[i].name();
        code += ";";
    }
    code += m_code;
    if (m_outputs.size() == 1) {
        code += ";return " + m_outputs[0].name() + ";";
    } else if (m_outputs.size() > 1) {
        
    }
    code += "})";
    return code;
}

const QString& MivarFunctionRelation::type() const noexcept {
    return "prog";
}
