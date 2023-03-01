#include <iostream>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>
#include "ModelLoader.h"

void ModelLoader::readModelData(std::shared_ptr<MivarModel>& model, const QDomNode& modelNode) {
    QDomNamedNodeMap attributes = modelNode.attributes();
    for (int i = 0; i < attributes.size(); i++) {
        QDomNode param = attributes.item(i);
        if (param.nodeName() == "id")
            model->setId(param.nodeValue());
        else if (param.nodeName() == "shortName")
            model->setName(param.nodeValue());
        else if (param.nodeName() == "description")
            model->setDescription(param.nodeValue());
    }
}

void ModelLoader::loadClassParams(std::shared_ptr<MivarClass>& mivarClass, const QDomNode& paramsNode) {
    for (int relIdx = 0; relIdx < paramsNode.childNodes().size(); relIdx++) {
        QDomNode curNode = paramsNode.childNodes().at(relIdx);
        if (curNode.nodeName() == "parameter") {
            std::shared_ptr<MivarParam> param = std::make_shared<MivarParam>();
            for (int attIdx = 0; attIdx < curNode.attributes().size(); attIdx++) {
                QDomNode attribute = curNode.attributes().item(attIdx);
                if (attribute.nodeName() == "id")
                    param->setId(attribute.nodeValue());
                else if (attribute.nodeName() == "shortName")
                    param->setName(attribute.nodeValue());
                else if (attribute.nodeName() == "description")
                    param->setDescription(attribute.nodeValue());
                else if (attribute.nodeName() == "type")
                    param->setType(attribute.nodeValue());
            }
            if(param->isCorrect())
                mivarClass->addParam(param);
        }
    }
}

void ModelLoader::loadRelations(std::shared_ptr<MivarModel>& model, const QDomNode& relationsNode) {
    for (int relIdx = 0; relIdx < relationsNode.childNodes().size(); relIdx++) {
        QDomNode curNode = relationsNode.childNodes().at(relIdx);
        if (curNode.nodeName() == "relation") {
            QString inputs;
            QString outputs;
            std::shared_ptr<MivarRelation> relation;
            QString id = "", name = "", description = "", type = "";
            for (int attIdx = 0; attIdx < curNode.attributes().size(); attIdx++) {
                QDomNode attribute = curNode.attributes().item(attIdx);
                if (attribute.nodeName() == "id")
                    id = attribute.nodeValue();
                else if (attribute.nodeName() == "shortName")
                    name = attribute.nodeValue();
                else if (attribute.nodeName() == "description")
                    description = attribute.nodeValue();
                else if (attribute.nodeName() == "relationType")
                    type = attribute.nodeValue();
                else if (attribute.nodeName() == "inObj")
                    inputs = attribute.nodeValue();
                else if (attribute.nodeName() == "outObj")
                    outputs = attribute.nodeValue();
            }
            if (id.size() && name.size()) {
                if (type == "constr")
                    relation = std::make_shared<MivarFunctionRelation>();
                else if (type == "simple")
                    relation = std::make_shared<MivarFunctionRelation>();
                else if (type == "ifclause")
                    relation = std::make_shared<MivarFunctionRelation>();
                else
                    relation = std::make_shared<MivarFunctionRelation>();
                relation->setId(id);
                relation->setName(name);
                relation->setDescription(description);
                QStringList params = inputs.split(";");
                for (const QString& curParam : params) {
                    QStringList paramData = curParam.split(":");
                    if (paramData.size() == 2) {
                        try {
                            MivarRelation::RelationParameter param(paramData[0], paramData[1]);
                            relation->addInput(param);
                        }
                        catch(...) {}
                    }
                }
                params = outputs.split(";");
                for (const QString& curParam : params) {
                    QStringList paramData = curParam.split(":");
                    if (paramData.size() == 2) {
                        try {
                            MivarRelation::RelationParameter param(paramData[0], paramData[1]);
                            relation->addOutput(param);
                        }
                        catch(...) {}
                    }
                }
                relation->setCode(curNode.childNodes().item(0).nodeValue());
                auto error = relation->errorDetails();
                if (error.line == -1)
                    model->addRelation(relation);
                else
                    std::cout << "Error <" << name.toStdString() << ">: " << error.line << ". " << error.msg.toStdString() << std::endl;
            }
        }
    } 
}

void ModelLoader::bindRuleParams(std::shared_ptr<MivarRule>& rule, const QString& params) {
    QStringList paramList = params.split(";");
    for (const QString& curParam : paramList) {
        QStringList paramData = curParam.split(":");
        if (paramData.size() == 2) {
            rule->bindParam(paramData[0], paramData[1]);
        }
    }
}

void ModelLoader::loadRules(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& mivarClass, const QDomNode& rulesNode) {
    for (int relIdx = 0; relIdx < rulesNode.childNodes().size(); relIdx++) {
        QDomNode curNode = rulesNode.childNodes().at(relIdx);
        if (curNode.nodeName() == "rule") {
            try {
                QString id = "", name = "", description = "", relationId = "", inpBinds = "", outBinds = "";
                for (int attIdx = 0; attIdx < curNode.attributes().size(); attIdx++) {
                    QDomNode attribute = curNode.attributes().item(attIdx);
                    if (attribute.nodeName() == "id")
                        id = attribute.nodeValue();
                    else if (attribute.nodeName() == "shortName")
                        name = attribute.nodeValue();
                    else if (attribute.nodeName() == "description")
                        description = attribute.nodeValue();
                    else if (attribute.nodeName() == "initId")
                        inpBinds = attribute.nodeValue();
                    else if (attribute.nodeName() == "resultId")
                        outBinds = attribute.nodeValue();
                    else if (attribute.nodeName() == "relation")
                        relationId = attribute.nodeValue();
                }
                std::shared_ptr<MivarRelation> ruleRelation = model->getRelation(relationId);
                std::shared_ptr<MivarRule> rule = std::make_shared<MivarRule>(ruleRelation);
                rule->setId(id);
                rule->setName(name);
                rule->setDescription(description);
                bindRuleParams(rule, inpBinds);
                bindRuleParams(rule, outBinds);
                mivarClass->addRule(rule);
            } catch (std::invalid_argument e) {
                std::cout << "error: " << e.what() << std::endl;
            }
        }
    }
}

void ModelLoader::loadSubclasses(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& parentClass, const QDomNode& classesNode) {
    for (int i = 0; i < classesNode.childNodes().size(); i++) {
        QDomNode node = classesNode.childNodes().at(i);
        if (node.nodeName() == "class") {
            std::shared_ptr<MivarClass> child = std::make_shared<MivarClass>();
            loadClass(model, child, node);
            parentClass->addSubclass(child);
        }
    }
}

void ModelLoader::loadClass(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& mivarClass, const QDomNode& classesNode){
    for (int attIdx = 0; attIdx < classesNode.attributes().size(); attIdx++) {
        QDomNode attribute = classesNode.attributes().item(attIdx);
        if (attribute.nodeName() == "id")
            mivarClass->setId(attribute.nodeValue());
        else if (attribute.nodeName() == "shortName")
            mivarClass->setName(attribute.nodeValue());
        else if (attribute.nodeName() == "description")
            mivarClass->setDescription(attribute.nodeValue());
    }
    for (int i = 0; i < classesNode.childNodes().size(); i++) {
        QDomNode node = classesNode.childNodes().at(i);
        if (node.nodeName() == "classes")
            loadSubclasses(model, mivarClass, node);
        else if (node.nodeName() == "rules")
            loadRules(model, mivarClass, node);
        else if (node.nodeName() == "parameters")
            loadClassParams(mivarClass, node);
    }
}

std::shared_ptr<MivarModel> ModelLoader::load(const QString &path)
{
    QFile inp(path);
    if (inp.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        doc.setContent(&inp);
        QDomElement root = doc.documentElement();
        if (root.tagName() == "model") {
            std::shared_ptr<MivarModel> model = std::make_shared<MivarModel>();
            readModelData(model, root);
            QDomNode rootClassNode;
            for (int i = 0; i < root.childNodes().size(); i++) {
                QDomNode node = root.childNodes().at(i);
                if (node.nodeName() == "class") {
                    rootClassNode = node;
                }
                else if (node.nodeName() == "relations")
                    loadRelations(model, node);
            }
            if (!rootClassNode.isNull()) {
                std::shared_ptr<MivarClass> rootClass = std::make_shared<MivarClass>();
                loadClass(model, rootClass, rootClassNode);
                model->setModelClass(rootClass);
            } else { /* ОБРАБОТКА ОТСУТСТВИЯ КЛАССОВ */ }
            return model;
        }
    }
    return std::make_shared<MivarModel>("id-", "name-", "desc-");
}

void ModelLoader::save(const MivarModel& model, const QString& path) {
}
