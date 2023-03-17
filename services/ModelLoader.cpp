#include <QDebug>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>
#include <QTextStream>
#include "ModelLoader.h"

const QString ModelLoader::MODEL_TAG = "model";
const QString ModelLoader::CLASS_TAG = "class";
const QString ModelLoader::CLASS_LIST_TAG = "classes";
const QString ModelLoader::PARAM_TAG = "parameter";
const QString ModelLoader::PARAM_LIST_TAG = "parameters";
const QString ModelLoader::RELATION_TAG = "relation";
const QString ModelLoader::RELATION_LIST_TAG = "relations";
const QString ModelLoader::RULE_TAG = "rule";
const QString ModelLoader::RULE_LIST_TAG = "rules";
const QString ModelLoader::CONSTRAINT_LIST_TAG = "constraints";

const QString ModelLoader::ID_ATTRIBUTE = "id";
const QString ModelLoader::NAME_ATTRIBUTE = "shortName";
const QString ModelLoader::DESCRIPTION_ATTRIBUTE = "description";
const QString ModelLoader::PARAM_TYPE_ATTRIBUTE = "type";
const QString ModelLoader::PARAM_DEFAULT_VAL_ATTRIBUTE = "defaultValue";
const QString ModelLoader::RELATION_TYPE_ATTRIBUTE = "relationType";
const QString ModelLoader::RELATION_INPUTS_ATTRIBUTE = "inObj";
const QString ModelLoader::RELATION_OUTPUTS_ATTRIBUTE = "outObj";
const QString ModelLoader::RULE_INPUTS_ATTRIBUTE = "initId";
const QString ModelLoader::RULE_OUTPUTS_ATTRIBUTE = "resultId";
const QString ModelLoader::RULE_RELATION_ATTRIBUTE = "relation";

const QString ModelLoader::RELATION_TYPE_SIMPLE = "simple";
const QString ModelLoader::RELATION_TYPE_CONSTRAINT = "constr";
const QString ModelLoader::RELATION_TYPE_IFCLAUSE = "ifclause";
const QString ModelLoader::RELATION_TYPE_FUNCTION = "prog";

void ModelLoader::readModelData(std::shared_ptr<MivarModel>& model, const QDomNode& modelNode) {
    QDomNamedNodeMap attributes = modelNode.attributes();
    for (int i = 0; i < attributes.size(); i++) {
        QDomNode param = attributes.item(i);
        if (param.nodeName() == ID_ATTRIBUTE)
            model->setId(param.nodeValue());
        else if (param.nodeName() == NAME_ATTRIBUTE)
            model->setName(param.nodeValue());
        else if (param.nodeName() == DESCRIPTION_ATTRIBUTE)
            model->setDescription(param.nodeValue());
    }
}

void ModelLoader::loadClassParams(std::shared_ptr<MivarClass>& mivarClass, const QDomNode& paramsNode) {
    for (int relIdx = 0; relIdx < paramsNode.childNodes().size(); relIdx++) {
        QDomNode curNode = paramsNode.childNodes().at(relIdx);
        if (curNode.nodeName() == PARAM_TAG) {
            std::shared_ptr<MivarParam> param = std::make_shared<MivarParam>();
            QString defaultVal = "";
            for (int attIdx = 0; attIdx < curNode.attributes().size(); attIdx++) {
                QDomNode attribute = curNode.attributes().item(attIdx);
                if (attribute.nodeName() == ID_ATTRIBUTE)
                    param->setId(attribute.nodeValue());
                else if (attribute.nodeName() == NAME_ATTRIBUTE)
                    param->setName(attribute.nodeValue());
                else if (attribute.nodeName() == DESCRIPTION_ATTRIBUTE)
                    param->setDescription(attribute.nodeValue());
                else if (attribute.nodeName() == PARAM_TYPE_ATTRIBUTE)
                    param->setType(attribute.nodeValue());
                else if (attribute.nodeName() == PARAM_DEFAULT_VAL_ATTRIBUTE)
                    defaultVal = attribute.nodeValue();
            }
            if (defaultVal.size())
                param->setDefaultValue(defaultVal);
            if(param->isCorrect())
                mivarClass->addParam(param);
        }
    }
}

void ModelLoader::loadRelations(std::shared_ptr<MivarModel>& model, const QDomNode& relationsNode) {
    for (int relIdx = 0; relIdx < relationsNode.childNodes().size(); relIdx++) {
        QDomNode curNode = relationsNode.childNodes().at(relIdx);
        if (curNode.nodeName() == RELATION_TAG) {
            QString inputs;
            QString outputs;
            std::shared_ptr<MivarRelation> relation;
            QString id = "", name = "", description = "", type = "";
            for (int attIdx = 0; attIdx < curNode.attributes().size(); attIdx++) {
                QDomNode attribute = curNode.attributes().item(attIdx);
                if (attribute.nodeName() == ID_ATTRIBUTE)
                    id = attribute.nodeValue();
                else if (attribute.nodeName() == NAME_ATTRIBUTE)
                    name = attribute.nodeValue();
                else if (attribute.nodeName() == DESCRIPTION_ATTRIBUTE)
                    description = attribute.nodeValue();
                else if (attribute.nodeName() == RELATION_TYPE_ATTRIBUTE)
                    type = attribute.nodeValue();
                else if (attribute.nodeName() == RELATION_INPUTS_ATTRIBUTE)
                    inputs = attribute.nodeValue();
                else if (attribute.nodeName() == RELATION_OUTPUTS_ATTRIBUTE)
                    outputs = attribute.nodeValue();
            }
            if (id.size() && name.size()) {
                if (type == RELATION_TYPE_SIMPLE)
                    relation = std::make_shared<MivarFunctionRelation>();
                else if (type == RELATION_TYPE_CONSTRAINT)
                    relation = std::make_shared<MivarFunctionRelation>();
                else if (type == RELATION_TYPE_IFCLAUSE)
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
                    qDebug() << "Error <" << name << ">: " << error.line << ". " << error.msg;
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
        if (curNode.nodeName() == RULE_TAG) {
            try {
                QString id = "", name = "", description = "", relationId = "", inpBinds = "", outBinds = "";
                for (int attIdx = 0; attIdx < curNode.attributes().size(); attIdx++) {
                    QDomNode attribute = curNode.attributes().item(attIdx);
                    if (attribute.nodeName() == ID_ATTRIBUTE)
                        id = attribute.nodeValue();
                    else if (attribute.nodeName() == NAME_ATTRIBUTE)
                        name = attribute.nodeValue();
                    else if (attribute.nodeName() == DESCRIPTION_ATTRIBUTE)
                        description = attribute.nodeValue();
                    else if (attribute.nodeName() == RULE_INPUTS_ATTRIBUTE)
                        inpBinds = attribute.nodeValue();
                    else if (attribute.nodeName() == RULE_OUTPUTS_ATTRIBUTE)
                        outBinds = attribute.nodeValue();
                    else if (attribute.nodeName() == RULE_RELATION_ATTRIBUTE)
                        relationId = attribute.nodeValue();
                }
                std::shared_ptr<MivarRelation> ruleRelation = model->getRelation(relationId);
                std::shared_ptr<MivarRule> rule = std::make_shared<MivarRule>();
                rule->bindRelation(ruleRelation);
                rule->setId(id);
                rule->setName(name);
                rule->setDescription(description);
                bindRuleParams(rule, inpBinds);
                bindRuleParams(rule, outBinds);
                mivarClass->addRule(rule);
            } catch (std::invalid_argument e) {
                qDebug() << "error: " << e.what();
            }
        }
    }
}

void ModelLoader::loadSubclasses(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& parentClass, const QDomNode& classesNode) {
    for (int i = 0; i < classesNode.childNodes().size(); i++) {
        QDomNode node = classesNode.childNodes().at(i);
        if (node.nodeName() == CLASS_TAG) {
            std::shared_ptr<MivarClass> child = std::make_shared<MivarClass>();
            loadClass(model, child, node);
            parentClass->addSubclass(child);
        }
    }
}

void ModelLoader::loadClass(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& mivarClass, const QDomNode& classesNode){
    for (int attIdx = 0; attIdx < classesNode.attributes().size(); attIdx++) {
        QDomNode attribute = classesNode.attributes().item(attIdx);
        if (attribute.nodeName() == ID_ATTRIBUTE)
            mivarClass->setId(attribute.nodeValue());
        else if (attribute.nodeName() == NAME_ATTRIBUTE)
            mivarClass->setName(attribute.nodeValue());
        else if (attribute.nodeName() == DESCRIPTION_ATTRIBUTE)
            mivarClass->setDescription(attribute.nodeValue());
    }
    for (int i = 0; i < classesNode.childNodes().size(); i++) {
        QDomNode node = classesNode.childNodes().at(i);
        if (node.nodeName() == CLASS_LIST_TAG)
            loadSubclasses(model, mivarClass, node);
        else if (node.nodeName() == RULE_LIST_TAG)
            loadRules(model, mivarClass, node);
        else if (node.nodeName() == PARAM_LIST_TAG)
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
        if (root.tagName() == MODEL_TAG) {
            std::shared_ptr<MivarModel> model = std::make_shared<MivarModel>();
            readModelData(model, root);
            QDomNode rootClassNode;
            for (int i = 0; i < root.childNodes().size(); i++) {
                QDomNode node = root.childNodes().at(i);
                if (node.nodeName() == CLASS_TAG) {
                    rootClassNode = node;
                }
                else if (node.nodeName() == RELATION_LIST_TAG)
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
    return std::make_shared<MivarModel>("", "", "");
}

void ModelLoader::saveParam(QDomDocument& doc, std::shared_ptr<MivarParam>& param, QDomNode& parentNode) {
    QString type;
    switch (param->type())
    {
    case MivarParam::PARAM_TYPE_DOUBLE :
        type = "double";
        break;
    case MivarParam::PARAM_TYPE_STRING :
        type = "string";
        break;
    default:
        type = "";
        break;
    }
    QDomElement paramNode = doc.createElement(PARAM_TAG);
    paramNode.setAttribute(ID_ATTRIBUTE, param->id());
    paramNode.setAttribute(NAME_ATTRIBUTE, param->name());
    paramNode.setAttribute(DESCRIPTION_ATTRIBUTE, param->description());
    paramNode.setAttribute(PARAM_DEFAULT_VAL_ATTRIBUTE, param->defaultValue());
    paramNode.setAttribute(PARAM_TYPE_ATTRIBUTE, type);
    parentNode.appendChild(paramNode);
}

void ModelLoader::saveRule(QDomDocument& doc, std::shared_ptr<MivarRule>& rule, QDomNode& parentNode) {
    const std::shared_ptr<MivarRelation> rel = rule->getBindetRelation();
    QString inputs = "";
    if (rel->inputs().size()) {
        inputs += rel->inputs()[0].name + ":" + rule->paramID(rel->inputs()[0].name);
        for(size_t i = 1; i < rel->inputs().size(); i++)
            inputs += ";" + rel->inputs()[i].name + ":" + rule->paramID(rel->inputs()[i].name);
    }
    QString outputs = "";
    if (rel->outputs().size()) {
        outputs += rel->outputs()[0].name + ":" + rule->paramID(rel->outputs()[0].name);
        for(size_t i = 1; i < rel->outputs().size(); i++)
            outputs += ";" + rel->outputs()[i].name + ":" + rule->paramID(rel->outputs()[i].name);
    }
    QDomElement ruleNode = doc.createElement(RULE_TAG);
    ruleNode.setAttribute(ID_ATTRIBUTE, rule->id());
    ruleNode.setAttribute(NAME_ATTRIBUTE, rule->name());
    ruleNode.setAttribute(DESCRIPTION_ATTRIBUTE, rule->description());
    ruleNode.setAttribute(RULE_INPUTS_ATTRIBUTE, inputs);
    ruleNode.setAttribute(RULE_OUTPUTS_ATTRIBUTE, outputs);
    ruleNode.setAttribute(RULE_RELATION_ATTRIBUTE, rel->id());
    parentNode.appendChild(ruleNode);
}

void ModelLoader::saveClass(QDomDocument &doc, std::shared_ptr<MivarClass> &mivarClass, QDomNode &parentNode) {
    QDomElement classNode = doc.createElement(CLASS_TAG);
    classNode.setAttribute(ID_ATTRIBUTE, mivarClass->id());
    classNode.setAttribute(NAME_ATTRIBUTE, mivarClass->name());
    classNode.setAttribute(DESCRIPTION_ATTRIBUTE, mivarClass->description());
    parentNode.appendChild(classNode);

    QDomElement paramsNode = doc.createElement(PARAM_LIST_TAG);
    classNode.appendChild(paramsNode);
    for(std::shared_ptr<MivarParam> param : mivarClass->params())
        saveParam(doc, param, paramsNode);

    QDomElement rules = doc.createElement(RULE_LIST_TAG);
    classNode.appendChild(rules);
    for(std::shared_ptr<MivarRule> rule : mivarClass->rules())
        saveRule(doc, rule, rules);
        
    QDomElement constraints = doc.createElement(CONSTRAINT_LIST_TAG);
    classNode.appendChild(constraints);

    QDomElement subclassesNode = doc.createElement(CLASS_LIST_TAG);
    classNode.appendChild(subclassesNode);
    for(std::shared_ptr<MivarClass> subclass : mivarClass->subclasses())
        saveClass(doc, subclass, subclassesNode);
}

void ModelLoader::saveRelation(QDomDocument& doc, std::shared_ptr<MivarRelation>& relation, QDomNode& parentNode){
    QString inputs = "";
    if (relation->inputs().size()) {
        inputs += relation->inputs()[0].name + ":" + relation->inputs()[0].type;
        for(size_t i = 1; i < relation->inputs().size(); i++)
            inputs += ";" + relation->inputs()[i].name + ":" + relation->inputs()[i].type;
    }
    QString outputs = "";
    if (relation->outputs().size()) {
        outputs += relation->outputs()[0].name + ":" + relation->outputs()[0].type;
        for(size_t i = 1; i < relation->outputs().size(); i++)
            outputs += ";" + relation->outputs()[i].name + ":" + relation->outputs()[i].type;
    }

    QDomElement relationNode = doc.createElement(RELATION_TAG);
    relationNode.setAttribute(ID_ATTRIBUTE, relation->id());
    relationNode.setAttribute(NAME_ATTRIBUTE, relation->name());
    relationNode.setAttribute(DESCRIPTION_ATTRIBUTE, relation->description());
    relationNode.setAttribute(RELATION_TYPE_ATTRIBUTE, relation->type());
    relationNode.setAttribute(RELATION_INPUTS_ATTRIBUTE, inputs);
    relationNode.setAttribute(RELATION_OUTPUTS_ATTRIBUTE, outputs);

    QDomText code = doc.createTextNode(relation->code());
    relationNode.appendChild(code);

    parentNode.appendChild(relationNode);
}

void ModelLoader::save(const std::shared_ptr<MivarModel>& model, const QString& path) {
    QFile xmlFile(path);
    if (!xmlFile.open(QFile::WriteOnly | QFile::Text )) {
        xmlFile.close();
    }
    QTextStream xmlContent(&xmlFile);

    QDomDocument modelDoc;
    QDomElement root = modelDoc.createElement(MODEL_TAG);
    modelDoc.appendChild(root);
    root.setAttribute("formatXmlVersion", "2.0");
    root.setAttribute(ID_ATTRIBUTE, model->id());
    root.setAttribute(NAME_ATTRIBUTE, model->name());
    if (model->description().size())
        root.setAttribute(DESCRIPTION_ATTRIBUTE, model->description());
    saveClass(modelDoc, model->modelClass(), root);

    QDomElement relationsNode = modelDoc.createElement(RELATION_LIST_TAG);
    root.appendChild(relationsNode);
    for(std::shared_ptr<MivarRelation> rel : model->relations())
        saveRelation(modelDoc, rel, relationsNode);

    xmlContent << modelDoc.toString();
}
