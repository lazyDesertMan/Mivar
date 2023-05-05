#ifndef PARAM_CALCULATOR_H
#define PARAM_CALCULATOR_H

#include <vector>
#include <map>
#include <set>
#include <models/MivarModel.h>
#include <models/MivarRule.h>

class ParamCalculator {
    std::set<QString> m_visitedID;

    template<typename T>
    using IDMap = std::map<QString, std::shared_ptr<T>>;

    void calcRule(std::shared_ptr<MivarRule> rule, std::map<QString, std::vector<QString>>& params);

    void ruleWaySearch(const IDMap<MivarRule>& rules,
        std::map<QString, std::vector<QString>>& params,
        const QString& id);
    void paramWaySearch(const IDMap<MivarRule>& rules,
        std::map<QString, std::vector<QString>>& params,
        const QString& id);

    void loadRules(IDMap<MivarRule>& rules, const std::shared_ptr<MivarClass>& mivarClass);
public:
    struct ParamCalcResult {
        QString id;
        std::vector<QString> values;

        ParamCalcResult(const QString& id, const std::vector<QString> vals) : id(id), values(vals) {}
    };

    std::vector<ParamCalcResult> findWays(
        const std::shared_ptr<MivarModel> model,
        std::map<QString, QString> inputVals,
        const std::vector<QString> searchingValsID
    );
};

#endif
