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
                else if (attribute.nodeName() == "inObj") {
                    inputs = attribute.nodeValue();
                    /*QString paramListStr = attribute.nodeValue();
                    QStringList params = paramListStr.split(";");
                    for (const QString& curParam : params) {
                        QStringList paramData = curParam.split(":");
                        if (paramData.size() == 2) {
                            try {
                                 param(paramData[0], paramData[1]);
                                relation->addInput(param);
                            }
                            catch(...) {}
                        }
                    }*/
                }
                else if (attribute.nodeName() == "outObj") {
                    outputs = attribute.nodeValue();
                    /*QString paramListStr = attribute.nodeValue();
                    QStringList params = paramListStr.split(";");
                    for (const QString& curParam : params) {
                        QStringList paramData = curParam.split(":");
                        if (paramData.size() == 2) {
                            try {
                                MivarRelation::RelationParameter param(paramData[0], paramData[1]);
                                relation->addOutput(param);
                            }
                            catch(...) {}
                        }
                    }*/
                }
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
                model.addRelation(relation);
            }
        }
    } 
}

void ModelLoader::loadSubclasses(MivarModel& model, const QDomNode& classesNode) {
}

void ModelLoader::loadClass(MivarModel& model, const QDomNode& classesNode) {
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
                if (node.nodeName() == "class")
                    loadClass(model, node);
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