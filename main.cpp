#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;

class GA{
public:
    int pSize;//种群大小
    int cLength; //染色体长度(5位二进制)
    double mRate; //变异概率
    int maxGen; //最大代数
    GA():pSize(10),cLength(5), mRate(0.01), maxGen(100){}

    //随机生成一个x值（即5位二进制串对应的整数）
    int randomChromosome() {
        return rand() % 32; // 生成[0, 31]之间的整数
    }
    //适应度函数 f(x) = x^2
    int fitness(int x) {
        return x * x;
    }
    //轮盘赌选择法
    int select(const vector<int>& population, const vector<int>& fitnessScores) {
        int totalFitness = 0;
        for (int score : fitnessScores){
            totalFitness += score;
        }

        int rand_point = rand() % totalFitness;
        int cumulativeFitness = 0;

        for (int i = 0; i < population.size(); i++) {
            cumulativeFitness += fitnessScores[i];
            if (cumulativeFitness >= rand_point) return population[i];
        }
        return population[0]; //防止异常
    }
    //单点交叉
    pair<int, int> crossover(int parent1, int parent2) {
        int crossoverPoint = rand() % cLength;//交叉点的位置（从右侧数）
        int mask = (1 << (crossoverPoint + 1)) - 1;//包括交叉点右侧变为1

        int child1 = (parent1 & mask) | (parent2 & ~mask);
        int child2 = (parent2 & mask) | (parent1 & ~mask);

        return {child1, child2};
    }
    //变异
    int mutate(int chromosome) {
        for (int i = 0; i < cLength; i++) {
            if ((double)rand() / RAND_MAX < mRate) {
                chromosome ^= (1 << i); // 翻转第i位
            }
        }
        return chromosome;
    }
    //遗传算法主函数
    void geneticAlgorithm() {
        srand(time(0));
        cout<<"遗传算法"<<endl;

        //初始化种群
        vector<int> population(pSize);
        for (int i = 0; i < pSize; i++) {
            population[i] = randomChromosome();
        }

        int generation = 0;
        while (generation < maxGen) {
            //计算适应度
            vector<int> fitnessScores(pSize);
            for (int i = 0; i < pSize; i++) {
                fitnessScores[i] = fitness(population[i]);
            }

            //输出当前代的最佳解
            int bestIndividual = population[0];  //假设第一个个体是最好的
            int bestFitness = fitnessScores[0];

            for (int i = 1; i < pSize; i++) {
                if (fitnessScores[i] > bestFitness) {
                    bestIndividual = population[i];
                    bestFitness = fitnessScores[i];
                }
            }
            //选择与交叉
            vector<int> newPopulation;
            while (newPopulation.size() < pSize) {
                int parent1 = select(population, fitnessScores);
                int parent2 = select(population, fitnessScores);
                auto [child1, child2] = crossover(parent1, parent2);
                newPopulation.push_back(mutate(child1));
                newPopulation.push_back(mutate(child2));
            }

            population = newPopulation;
            generation++;
        }

        //由于变异几率低，所以不进行变异

        //输出最终结果
        int bestIndividual = population[0];  //假设第一个个体是最好的

        for (int i = 1; i < pSize; i++) {
            if (fitness(population[i]) > fitness(bestIndividual)) {
                bestIndividual = population[i];
            }
        }

        cout << "函数最大值：" << fitness(bestIndividual) << endl;
    }
};

int main() {
    GA ga;
    ga.geneticAlgorithm();
    return 0;
}