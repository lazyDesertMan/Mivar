#include <boost/graph/tiernan_all_cycles.hpp>
#include "GraphService.h"

// Добавление пустой реализации для функции, требуемой в Boost graph_concepts.hpp, стр.459
namespace boost { void renumber_vertex_indices(const Graph&) {} }

QString GraphService::genNodeName(std::shared_ptr<MivarObject> parent, std::shared_ptr<MivarObject> child, QString prefix) {
    auto normalize = [](QString& str) -> QString& {
        return str.remove("\"").replace("\n", " ").remove("\r").replace(".", " ");
    };
    QString parentName = parent->name();
    QString childName = prefix + child->name();
    return normalize(parentName) + "." + normalize(childName);
}

void GraphService::loadParams(std::map<QString, size_t> &paramIndexList, std::shared_ptr<MivarClass> &mivarClass)
{
    for(std::shared_ptr<MivarParam> param : mivarClass->params())
        paramIndexList.insert({param->id(), paramIndexList.size()});
    for(std::shared_ptr<MivarClass> subclass : mivarClass->subclasses())
        loadParams(paramIndexList, subclass);
}

void GraphService::loadParams(std::map<QString, std::pair<QString, QString>>& paramList, std::shared_ptr<MivarClass>& mivarClass) {
    for(std::shared_ptr<MivarParam> param : mivarClass->params())
        paramList.insert({param->id(), {genNodeName(mivarClass, param), ""}});
    for(std::shared_ptr<MivarClass> subclass : mivarClass->subclasses())
        loadParams(paramList, subclass);
}

void GraphService::loadRules(std::map<QString, size_t>& ruleIndexList, std::shared_ptr<MivarClass>& mivarClass) {
    for(std::shared_ptr<MivarRule> rule : mivarClass->rules())
        ruleIndexList.insert({rule->id(), ruleIndexList.size()});
    for(std::shared_ptr<MivarClass> subclass : mivarClass->subclasses())
        loadRules(ruleIndexList, subclass);
}

void GraphService::loadRules(std::map<QString, std::pair<QString, QString>>& paramList, std::map<QString, std::pair<QString, QString>>& ruleList, std::shared_ptr<MivarClass>& mivarClass) {
    for(std::shared_ptr<MivarRule> rule : mivarClass->rules()) {
        QString inputs = "[";
        for(QString paramId : rule->inputsId()) {
            inputs += "\"" + paramList[paramId].first + "\",";
        }
        inputs += "]";
        const QString ruleName = genNodeName(mivarClass, rule, "[R]: ");
        for(QString paramId : rule->outputsId()) {
            paramList[paramId].second += "\"" + ruleName + "\",";
        }
        ruleList.insert({rule->id(), {ruleName, inputs}});
    }
    for(std::shared_ptr<MivarClass> subclass : mivarClass->subclasses())
        loadRules(paramList, ruleList, subclass);
}

void GraphService::bindParams(std::map<QString, size_t>& paramIndexList, std::map<QString, size_t>& ruleIndexList, Graph& graph, std::shared_ptr<MivarClass>& mivarClass) {
    size_t ruleDisplace =  paramIndexList.size();
    for(std::shared_ptr<MivarRule> rule : mivarClass->rules()) {
        for(QString paramId : rule->inputsId())
            boost::add_edge(paramIndexList[paramId], ruleIndexList[rule->id()] + ruleDisplace, graph);
        for(QString paramId : rule->outputsId())
            boost::add_edge(ruleIndexList[rule->id()] + ruleDisplace, paramIndexList[paramId], graph);
    }
    for(std::shared_ptr<MivarClass> subclass : mivarClass->subclasses())
        bindParams(paramIndexList, ruleIndexList, graph, subclass);
}

std::vector<CycleDetails> GraphService::findRuleCycles(std::shared_ptr<MivarModel>& model) {
    std::vector<CycleDetails> pathes;
    std::map<QString, size_t> params;
    std::map<QString, size_t> rules;
    loadParams(params, model->modelClass());
    loadRules(rules, model->modelClass());
    Graph gr(params.size() + rules.size());
    bindParams(params, rules, gr, model->modelClass());
    CycleFinder finder(params.size(), rules, pathes);
    tiernan_all_cycles(gr, finder);
    return pathes;
}

QString GraphService::ruleDependencyJson(std::shared_ptr<MivarModel>& model) {
    if (model.get() != nullptr) {
        QString code = "[";
        std::map<QString, std::pair<QString, QString>> paramDependecy;
        std::map<QString, std::pair<QString, QString>> ruleDependecy;
        loadParams(paramDependecy, model->modelClass());
        loadRules(paramDependecy, ruleDependecy, model->modelClass());
        for(auto it = paramDependecy.begin(); it != paramDependecy.end(); it++)
            code += "{\"name\":\"" + it->second.first + "\",\"imports\":[" + it->second.second + "]},";
        for(auto it = ruleDependecy.begin(); it != ruleDependecy.end(); it++)
            code += "{\"name\":\"" + it->second.first + "\",\"imports\":" + it->second.second + "},";
        code += "]";
        return code;
    }
    return "[{\"name\":\"no model\"}]";
}
