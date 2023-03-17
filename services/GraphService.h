#ifndef GRAPH_SERVICE_H
#define GRAPH_SERVICE_H

#include <vector>
#include <QString>
#include <QDebug>
#include <boost/graph/adjacency_list.hpp>
#include <models/MivarModel.h>

struct CycleDetails {
    std::vector<QString> way;
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>;

class GraphService {
    struct CycleFinder {
        CycleFinder(size_t params_cnt, std::map<QString, size_t>& rules, std::vector<CycleDetails>& pathes) : params_cnt(params_cnt), rules(rules), pathes(pathes) {}
        std::vector<CycleDetails>& pathes;
        size_t params_cnt;
        std::map<QString, size_t>& rules;

        template <typename Path>
        void cycle(const Path& path, Graph const& g) {
            CycleDetails cyclePath;
            auto indices = get(boost::vertex_index, g);
            for (auto node : path) {
                auto idx = get(indices, node);
                if (idx >= params_cnt) {
                    idx -= params_cnt;
                    for (auto rule : rules) 
                        if (rule.second == idx)
                            cyclePath.way.push_back(rule.first);
                }
            }
            if(cyclePath.way.size())
                pathes.push_back(cyclePath);
        };
    };

    static QString genNodeName(std::shared_ptr<MivarObject> parent, std::shared_ptr<MivarObject> child, QString prefix = "");
    static void loadParams(std::map<QString, size_t>& paramIndexList, std::shared_ptr<MivarClass>& mivarClass);
    static void loadParams(std::map<QString, std::pair<QString, QString>>& paramList, std::shared_ptr<MivarClass>& mivarClass);
    static void loadRules(std::map<QString, size_t>& ruleIndexList, std::shared_ptr<MivarClass>& mivarClass);
    static void loadRules(std::map<QString, std::pair<QString, QString>>& paramList, std::map<QString, std::pair<QString, QString>>& ruleList, std::shared_ptr<MivarClass>& mivarClass);
    /**
     * @brief Добавление путей между графами и параметрами
     * 
     * @warning Первыми считаются узлы параметров, потом следуют узлы правил
     * 
     * @param paramIndexList Список параметров
     * @param ruleIndexList Список правил
     * @param graph Граф
     * @param mivarClass Класс 
     */
    static void bindParams(std::map<QString, size_t>& paramIndexList, std::map<QString, size_t>& ruleIndexList, Graph& graph, std::shared_ptr<MivarClass>& mivarClass);
public:
    static std::vector<CycleDetails> findRuleCycles(std::shared_ptr<MivarModel>& model);
    static QString ruleDependencyJson(std::shared_ptr<MivarModel>& model);
};

#endif
