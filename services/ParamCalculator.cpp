#include <QDebug>
#include "SingleJSEngine.h"
#include "ParamCalculator.h"

void ParamCalculator::calcRule(std::shared_ptr<MivarRule> rule, std::map<QString, std::vector<QString>> &params) {
    std::shared_ptr<MivarRelation> relation = rule->getBindetRelation();
    QJSEngine& engine = SingleJSEngine::engine();
    QJSValue func = engine.evaluate(rule->getBindetRelation()->toJSFunction());
    QJSValueList inp;
    for(auto& param : relation->inputs()) {
        QString paramID = rule->paramID(param.name);
        if (param.type == "double")
            inp << params[paramID][0].toDouble();  // TODO: Потом тут надо будет формировать всё сочетания входных параметров, а не брать первое значение
        else
            inp << params[paramID][0];
    }
    QJSValue funcResult = func.call(inp);
    int outSize = funcResult.property("length").toInt();
    const std::vector<MivarRelation::RelationParameter>& outNames = relation->outputs();
    for(int i = 0; i < outSize; i++) {
        QString paramID = rule->paramID(outNames[i].name);
        if (params.find(paramID) != params.end())
            params[paramID].push_back(funcResult.property(i).toString());
        else
            params.insert({paramID, {funcResult.property(i).toString()}});
    }
}

void ParamCalculator::ruleWaySearch(
    const IDMap<MivarRule> &rules,
    std::map<QString, std::vector<QString>> &params,
    const QString &id)
{
    const std::shared_ptr<MivarRule>& rule = rules.at(id);
    m_visitedID.insert(id);
    const std::vector<QString> inputs = rule->inputsId();
    for (auto& paramID : inputs)
        paramWaySearch(rules, params, paramID);
    for (auto& paramID : inputs)
        if(params.find(paramID) == params.end() || params[paramID].size() == 0)
            return;
    calcRule(rule, params);
}

void ParamCalculator::paramWaySearch(
    const IDMap<MivarRule> &rules,
    std::map<QString, std::vector<QString>> &params,
    const QString &id)
{
    //qDebug() << "visit param: " << id;
    for (auto& rule : rules) {
        auto outs = rule.second->outputsId();
        if (m_visitedID.find(rule.first) == m_visitedID.end() && std::find(outs.begin(), outs.end(), id) != outs.end()) {
            ruleWaySearch(rules, params, rule.second->id());
        }
    }
}

void ParamCalculator::loadRules(IDMap<MivarRule> &rules, const std::shared_ptr<MivarClass> &mivarClass) {
    for (auto& rule : mivarClass->rules())
        rules.insert({rule->id(), rule});
    for (auto& subclass : mivarClass->subclasses())
        loadRules(rules, subclass);
}

std::vector<QString> ParamCalculator::findWays(
    const std::shared_ptr<MivarModel> model,
    std::map<QString, QString> inputVals,
    std::vector<QString> outputValsID)
{
    m_visitedID.clear();
    IDMap<MivarRule> rules;
    loadRules(rules, model->modelClass());
    std::map<QString, std::vector<QString>> params;
    for(auto param : inputVals)
        params.insert({param.first, {param.second}});
    for(QString& paramID : outputValsID) {
        paramWaySearch(rules, params, paramID);
    }
    for(auto param : outputValsID)
        qDebug() << params[param];
    return std::vector<QString>();
}
