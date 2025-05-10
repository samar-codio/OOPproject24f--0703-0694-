#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <limits>
#include <windows.h>
using namespace std;

const int MAX_RESOURCES = 4;
const int MAX_MARKET_ITEMS = 4;
const int MAX_EVENTS = 5;
const int MAX_LEADERS = 3;

class Population;             // all the classes which can be used 
class Army;
class Economy;
class Politics;
class Bank;
class Market;
class ResourceManager;
class EventSystem;
class GameIO;
class Kingdom;

template <typename T>             // all the inventory system 
class Inventory {
private:
    T quantity;
public:
    Inventory() : quantity(0) {}
    Inventory(T initialQty) : quantity(initialQty) {}

    void set(T q) { quantity = q; }
    T get() const { return quantity; }
    void add(T amount) { quantity += amount; }
    bool remove(T amount) {
        if (quantity >= amount) {
            quantity -= amount;
            return true;
        }
        return false;
    }
};

class Leader {
protected:
    string name;
    int leaderScore;
public:
    Leader(const string& leaderName, int score) : name(leaderName), leaderScore(score) {}
    virtual ~Leader() {}

    string getName() const { return name; }
    int getLeadershipScore() const { return leaderScore; }
    virtual void policyEffect() = 0; // Pure virtual function
    virtual string getTitle() const = 0;
};

class King : public Leader {
private:
    bool corrupt;
    int taxMultiplier;
public:
    King(const string& name, int score) : Leader(name, score), corrupt(false), taxMultiplier(1) {}

    void setCorrupt(bool isCorrupt) { corrupt = isCorrupt; }
    bool isCorrupt() const { return corrupt; }

    void policyEffect() override;
    string getTitle() const override { return "King"; }

    int collectTaxes();
    bool declareWar();
};

class Commander : public Leader {
private:
    int strategyLevel;
public:
    Commander(const string& name, int score) : Leader(name, score), strategyLevel(1) {}

    void policyEffect() override;
    string getTitle() const override { return "Commander"; }

    bool issueCommand();
    void improveStrategy() { strategyLevel++; }
    int getStrategyLevel() const { return strategyLevel; }
};

class GuildLeader : public Leader {
private:
    int tradeInfluence;
public:
    GuildLeader(const string& name, int score) : Leader(name, score), tradeInfluence(1) {}

    void policyEffect() override;
    string getTitle() const override { return "Guild Leader"; }

    bool enforceTradePolicy();
    int getTradeInfluence() const { return tradeInfluence; }
    void improveInfluence() { tradeInfluence++; }
};

class Population {
private:
    int peasants;
    int merchants;
    int nobles;
    int military;
    int growthRate;

public:
    Population();

    void updateGrowth();
    void handlePlague();
    void adjustPopulationAfterWar(int casualties);
    void displayStats() const;

    int getTotalPopulation() const { return peasants + merchants + nobles + military; }
    int getPeasants() const { return peasants; }
    int getMerchants() const { return merchants; }
    int getNobles() const { return nobles; }
    int getMilitary() const { return military; }

    void addPeasants(int amount) { peasants += amount; }
    void addMerchants(int amount) { merchants += amount; }
    void addNobles(int amount) { nobles += amount; }
    void addMilitary(int amount) { military += amount; }

    void removePeasants(int amount) { peasants = (peasants - amount > 0) ? peasants - amount : 0; }
    void removeMerchants(int amount) { merchants = (merchants - amount > 0) ? merchants - amount : 0; }
    void removeNobles(int amount) { nobles = (nobles - amount > 0) ? nobles - amount : 0; }
    void removeMilitary(int amount) { military = (military - amount > 0) ? military - amount : 0; }
};

class Army {
private:
    int soldiers;
    int morale;
    bool paid;
    int weaponQuality;
    int battleExperience;

public:
    Army();

    void trainTroops();
    void updateMorale();
    bool paySoldiers(int amount);
    int goToWar();
    void displayStats() const;

    int getSoldiers() const { return soldiers; }
    int getMorale() const { return morale; }
    bool isPaid() const { return paid; }
    int getWeaponQuality() const { return weaponQuality; }

    void setSoldiers(int amount) { soldiers = amount; }
    void setMorale(int amount) { morale = amount; }
    void setPaid(bool status) { paid = status; }
    void improveWeapons() { weaponQuality++; }
};

class Economy {
private:
    double treasury;
    double taxRate;
    double inflationRate;

public:
    Economy();

    bool collectTaxes(Population* population, King* king);
    bool payWages(Population* population, Army* army);
    void triggerMarketCrash();
    void displayStats() const;

    double getTreasury() const { return treasury; }
    double getTaxRate() const { return taxRate; }
    double getInflationRate() const { return inflationRate; }

    void setTreasury(double amount) { treasury = amount; }
    void setTaxRate(double rate) { taxRate = rate; }
    void addFunds(double amount) { treasury += amount; }
    bool removeFunds(double amount);
};

class Bank {
private:
    double loanAmount;
    double interestRate;
    bool defaulted;
    int loanTime;

public:
    Bank();

    bool giveLoan(Economy* economy, double amount);
    bool repayLoan(Economy* economy);
    void audit();
    void displayStats() const;

    double getLoanAmount() const { return loanAmount; }
    double getInterestRate() const { return interestRate; }
    bool hasDefaulted() const { return defaulted; }
    int getLoanTerm() const { return loanTime; }

    void setLoanAmount(double amount) { loanAmount = amount; }
    void setInterestRate(double rate) { interestRate = rate; }
    void setDefaulted(bool status) { defaulted = status; }
};

class ResourceBase {
public:
    virtual int getResource(int index) const = 0;
    virtual void setResource(int index, int amount) = 0;
    virtual void addResource(int index, int amount) = 0;
    virtual bool removeResource(int index, int amount) = 0;
    virtual const string& getResourceName(int index) const = 0;
    virtual void displayStats() const = 0;
    virtual ~ResourceBase() {}
};

class ResourceManager : public ResourceBase {
private:
    Inventory<int> resources[MAX_RESOURCES];
    const string resourceNames[MAX_RESOURCES] = { "Food", "Wood", "Stone", "Gold" };

public:
    ResourceManager();

    void distributeResources(Population* population);
    bool handleShortages(Population* population);
    void displayStats() const override;

    int getResource(int index) const override { return resources[index].get(); }
    void setResource(int index, int amount) override { resources[index].set(amount); }
    void addResource(int index, int amount) override { resources[index].add(amount); }
    bool removeResource(int index, int amount) override { return resources[index].remove(amount); }
    const string& getResourceName(int index) const override { return resourceNames[index]; }

    int getFood() const { return getResource(0); }
    int getWood() const { return getResource(1); }
    int getStone() const { return getResource(2); }
    int getGold() const { return getResource(3); }

    void setFood(int amount) { setResource(0, amount); }
    void setWood(int amount) { setResource(1, amount); }
    void setStone(int amount) { setResource(2, amount); }
    void setGold(int amount) { setResource(3, amount); }

    void addFood(int amount) { addResource(0, amount); }
    void addWood(int amount) { addResource(1, amount); }
    void addStone(int amount) { addResource(2, amount); }
    void addGold(int amount) { addResource(3, amount); }

    bool removeFood(int amount) { return removeResource(0, amount); }
    bool removeWood(int amount) { return removeResource(1, amount); }
    bool removeStone(int amount) { return removeResource(2, amount); }
    bool removeGold(int amount) { return removeResource(3, amount); }
};

class Market {
private:
    double prices[MAX_MARKET_ITEMS];
    string itemName[MAX_MARKET_ITEMS];
    int tradePrice;

public:
    Market();

    void updatePrices();
    bool trade(ResourceManager* resources, Economy* economy, int resourceIndex, int amount, bool buying);
    void triggerSmuggling(ResourceManager* resources);
    void displayStats() const;

    double getPrice(int index) const { return prices[index]; }
    void setPrice(int index, double price) { prices[index] = price; }
    string getItemName(int index) const { return itemName[index]; }
    int getTradingVolume() const { return tradePrice; }
};

class Politics {
private:
    int stability;
    bool coupAttempted;
    string currentKingName;

public:
    Politics();

    King* electKing(const string& name);
    void applyBribery(Economy* economy);
    bool triggerCoup(Economy* economy, Population* population);
    void displayStats() const;

    int getStability() const { return stability; }
    bool wasCoupAttempted() const { return coupAttempted; }
    string getCurrentKingName() const { return currentKingName; }

    void setStability(int value) { stability = value; }
    void setCoupAttempted(bool attempted) { coupAttempted = attempted; }
    void setCurrentKingName(const string& name) { currentKingName = name; }
};

class EventSystem {
private:
    string eventNames[MAX_EVENTS];
    int eventPossibility[MAX_EVENTS];

public:
    EventSystem();

    int randomEvent();
    void handleEvent(int eventId, Kingdom* kingdom);
    void displayEvent(int eventId) const;
};

class GameIO {
public:
    bool saveToFile(const string& filename, Kingdom* kingdom);
    bool loadFromFile(const string& filename, Kingdom* kingdom);
    void saveScore(const string& playerName, int score);
    void displayHighScores() const;
};

class Kingdom {
private:
    string kingdomName;
    int turnNumber;
    int score;

    Population* population;
    Army* army;
    Economy* economy;
    Politics* politics;
    Bank* bank;
    Market* market;
    ResourceManager* resources;
    EventSystem* events;
    GameIO* gameIO;

    King* king;
    Commander* commander;
    GuildLeader* guildLeader;

public:
    Kingdom(const string& name);
    ~Kingdom();

    void runTurn();
    void displayStats() const;
    bool saveGame(const string& filename);
    bool loadGame(const string& filename);
    void gameOver();

    Population* getPopulation() const { return population; }
    Army* getArmy() const { return army; }
    Economy* getEconomy() const { return economy; }
    Politics* getPolitics() const { return politics; }
    Bank* getBank() const { return bank; }
    Market* getMarket() const { return market; }
    ResourceManager* getResources() const { return resources; }
    EventSystem* getEvents() const { return events; }
    GameIO* getGameIO() const { return gameIO; }

    King* getKing() const { return king; }
    Commander* getCommander() const { return commander; }
    GuildLeader* getGuildLeader() const { return guildLeader; }

    string getKingdomName() const { return kingdomName; }
    int getTurnNumber() const { return turnNumber; }
    int getScore() const { return score; }

    
    void setKing(King* newKing) { delete king; king = newKing; }
    void setCommander(Commander* newCommander) { delete commander; commander = newCommander; }
    void setGuildLeader(GuildLeader* newGuildLeader) { delete guildLeader; guildLeader = newGuildLeader; }
    void addScore(int points) { score += points; }
};


void waitKey();
void clearScreen();
void printStrongholdBanner();
void displayMenu();
void displayGameMenu();
void displayTradeMenu(Kingdom* kingdom);
void displayMilitaryMenu(Kingdom* kingdom);
void displayFinancialMenu(Kingdom* kingdom);
void handleTradeMenu(Kingdom* kingdom);
void handleMilitaryMenu(Kingdom* kingdom);
void handleFinancialMenu(Kingdom* kingdom);
void playGame(Kingdom* kingdom);