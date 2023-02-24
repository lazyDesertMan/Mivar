#include <iostream>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>
#include "ModelLoader.h"

void ModelLoader::readModelData(MivarModel& model, const QDomNode& modelNode)
{
    QDomNamedNodeMap attributes = modelNode.attributes();
    for (int i = 0; i < attributes.size(); i++) {
        QDomNode param = attributes.item(i);
        if (param.nodeName() == "id")
            model.setId(param.nodeValue());
        else if (param.nodeName() == "shortName")
            model.setName(param.nodeValue());
        else if (param.nodeName() == "description")
            model.setDescription(param.nodeValue());
    }
}

void ModelLoader::loadRelations(MivarModel& model, const QDomNode& relationsNode) {
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
                    model.addRelation(relation);
                else
                    std::cout << "Error <" << name.toStdString() << ">: " << error.line << ". " << error.msg.toStdString() << std::endl;
            }
        }
    } 
}

void ModelLoader::loadSubclasses(MivarClass& parentClass, const QDomNode& classesNode) {
    for (int i = 0; i < classesNode.childNodes().size(); i++) {
        QDomNode node = classesNode.childNodes().at(i);
        if (node.nodeName() == "class") {
            MivarClass child;
            loadClass(child, node);
            parentClass.addSubclass(child);
        }
    }
}

void ModelLoader::loadClass(MivarClass& mivarClass, const QDomNode& classesNode){
    for (int attIdx = 0; attIdx < classesNode.attributes().size(); attIdx++) {
        QDomNode attribute = classesNode.attributes().item(attIdx);
        if (attribute.nodeName() == "id")
            mivarClass.setId(attribute.nodeValue());
        else if (attribute.nodeName() == "shortName")
            mivarClass.setName(attribute.nodeValue());
        else if (attribute.nodeName() == "description")
            mivarClass.setDescription(attribute.nodeValue());
    }
    for (int i = 0; i < classesNode.childNodes().size(); i++) {
        QDomNode node = classesNode.childNodes().at(i);
        if (node.nodeName() == "classes")
            loadSubclasses(mivarClass, node);
    }
}

MivarModel ModelLoader::load(const QString &path)
{
    QFile inp(path);
    if (inp.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        doc.setContent(&inp);
        QDomElement root = doc.documentElement();
        if (root.tagName() == "model") {
            MivarModel model;
            readModelData(model, root);
            for (int i = 0; i < root.childNodes().size(); i++) {
                QDomNode node = root.childNodes().at(i);
                if (node.nodeName() == "class") {
                    MivarClass rootClass;
                    loadClass(rootClass, node);
                    model.setModelClass(rootClass);
                }
                else if (node.nodeName() == "relations")
                    loadRelations(model, node);
            }
            return model;
        }
    }
    return MivarModel("id-", "name-", "desc-");
}

void ModelLoader::save(const MivarModel &model, const QString &path)
{
}
