#include "gameoop.h"
void King::policyEffect() {

    if (leaderScore > 7) {
        taxMultiplier = 2;
    }
    else if (leaderScore > 3) {
        taxMultiplier = 1;
    }
    else {
        taxMultiplier = 0;
        corrupt = true;
    }
}

int King::collectTaxes() {
    return taxMultiplier * (corrupt ? -1 : 1);
}
bool King::declareWar() {
    return leaderScore > 5;
}

void Commander::policyEffect() {
    strategyLevel = leaderScore / 2;
}

bool Commander::issueCommand() {
    return (rand() % 10) < strategyLevel;
}

void GuildLeader::policyEffect() {
    tradeInfluence = leaderScore / 3 + 1;
}

bool GuildLeader::enforceTradePolicy() {
    return (rand() % 10) < tradeInfluence;
}

Population::Population() {
    peasants = 100;
    merchants = 20;
    nobles = 5;
    military = 10;
    growthRate = 5;
}

void Population::updateGrowth() {
    int growth = getTotalPopulation() * growthRate / 100;

    addPeasants(growth * 7 / 10);
    addMerchants(growth * 2 / 10);
    addNobles(growth * 1 / 20);
    addMilitary(growth * 1 / 20);
}

void Population::handlePlague() {

    int casualties = getTotalPopulation() / 10;
    removePeasants(casualties * 7 / 10);
    removeMerchants(casualties * 2 / 10);
    removeNobles(casualties / 20);
    removeMilitary(casualties / 20);
    cout << "Plague struck! " << casualties << " people died.\n";
}

void Population::adjustPopulationAfterWar(int casualties) {
    removeMilitary(casualties * 9 / 10);
    removePeasants(casualties * 1 / 10);
    cout  << casualties << " people killed .\n";
}

void Population::displayStats() const {
    cout << "===== POPULATION =====\n";
    cout << "Total Population: " << getTotalPopulation() << "\n";
    cout << ". Peasants: " << peasants << "\n";
    cout << ". Merchants: " << merchants << "\n";
    cout << ". Nobles: " << nobles << "\n";
    cout << ". Military: " << military << "\n";
    cout << ". Growth Rate: " << growthRate << "%\n";
}

Army::Army() {
    soldiers = 50;
    morale = 70;
    paid = true;
    weaponQuality = 1;
    battleExperience = 0;
}

void Army::trainTroops() {
    if (morale >= 50 && paid) {
        int solnum;
        cout << "How many soldiers: ";
        cin >> solnum;

        cout << "Troops training is ongoing. Morale will be increased to " << morale << endl;

      
        Sleep(solnum * 100);

        
        morale += 5;
        battleExperience++;
        cout << "Training completed! New morale: " << morale << endl;
        waitKey();
    }
    else {
        cout << "Training failed  low money or morale" << endl;
    }
}

void Army::updateMorale() {
    if (!paid) {
        morale -= 10;
    }
    morale = (morale < 0) ? 0 : (morale > 100) ? 100 : morale;
}

bool Army::paySoldiers(int amount) {
    if (amount >= soldiers) {
        paid = true;
        morale += 5;
        return true;
    }
    else {
        paid = false;
        morale -= 10;
        return false;
    }
}

int Army::goToWar() {
    if (soldiers < 10) {
        cout << "Not enough soldiers\n";
        return 0;
    }
    int strength = soldiers * (morale / 100) * (paid ? 1.5 : 0.5) * weaponQuality;
    int enemyStrength = rand() % (strength * 2) + strength / 2;

    cout << "Battle strength: " << strength << " vs Enemy: " << enemyStrength << "\n";

    int deaths = 0;
    if (strength > enemyStrength) {
        deaths = soldiers / 10;
        battleExperience += 2;
        morale += 10;
        cout << "Victory-> lost " << deaths << " soldiers.\n";
    }
    else {
        deaths = soldiers / 3;
        morale -= 20;
        cout << "Defeat-> lost " << deaths << " soldiers.\n";
    }

    soldiers -= deaths;
    return deaths;
}

void Army::displayStats() const {
    cout << "===== ARMY =====\n";
    cout << "Soldiers: " << soldiers << "\n";
    cout << "Morale: " << morale << "%\n";
    cout << "Paid: " << (paid ? "Yes" : "No") << "\n";
    cout << "Weapon Quality: " << weaponQuality << "\n";
    cout << "Battle Experience: " << battleExperience << "\n";
}

Economy::Economy() {
    treasury = 1000.0;
    taxRate = 10.0;
    inflationRate = 1.0;
}

bool Economy::collectTaxes(Population* population, King* king) {
    int taxMultiplier = king->collectTaxes();
    double collectedAmount = population->getTotalPopulation() * taxRate * taxMultiplier / 100.0;

    if (collectedAmount > 0) {
        treasury += collectedAmount;
        cout << "Taxes collected: " << collectedAmount << " gold.\n";
        return true;
    }
    else if (collectedAmount < 0) {
        treasury += collectedAmount;
        cout << "The corrupt king stole " << -collectedAmount << " gold from treasury!\n";
        return false;
    }

    cout << "No taxes collected.\n";
    return false;
}

bool Economy::payWages(Population* population, Army* army) {
    double sallary = population->getMilitary() * 2.0;

    if (treasury >= sallary) {
        treasury -= sallary;
        army->paySoldiers(sallary);
        cout << "Military sallary paid: " << sallary << " gold.\n";
        return true;
    }

    cout << "Cannot pay military sallary morale down \n";
    army->setPaid(false);
    return false;
}

void Economy::triggerMarketCrash() {
    treasury = treasury * 0.7;
    inflationRate *= 1.5;
    cout << "Market crash money reduced to " << treasury << " gold\n";
}

void Economy::displayStats() const {
    cout << "===== ECONOMY =====\n";
    cout << "Treasury: " << treasury << " gold\n";
    cout << "Tax Rate: " << taxRate << "%\n";
    cout << "Inflation: " << inflationRate << "%\n";
}

bool Economy::removeFunds(double amount) {
    if (treasury >= amount) {
        treasury -= amount;
        return true;
    }
    return false;
}

Bank::Bank() {
    loanAmount = 0;
    interestRate = 10;
    defaulted = false;
    loanTime = 0;
}

bool Bank::giveLoan(Economy* economy, double amount) {
    if (loanAmount > 0) {
        cout << "You already have loan\n";
        return false;
    }

    loanAmount = amount;
    loanTime = 5;
    economy->addFunds(amount);
    cout << "Loan of " << amount << " gold granted with " << interestRate << " interest\n";
    return true;
}

bool Bank::repayLoan(Economy* economy) {
    double repaymentAmount = loanAmount * (1 + interestRate / 100);

    if (economy->removeFunds(repaymentAmount)) {
        cout << "Loan repaid " << repaymentAmount << " gold\n";
        loanAmount = 0;
        loanTime = 0;
        return true;
    }

    cout << "Cannot repay loan  " << repaymentAmount << " gold\n";
    return false;
}

void Bank::audit() {
    if (loanAmount > 0) {
        loanTime--;

        if (loanTime <= 0) {
            defaulted = true;
            cout << "Loan default! The bank will stop money.\n";
        }
        else {
            cout << loanTime << " turns remaining on loan of " << loanAmount << " gold.\n";
        }
    }
}

void Bank::displayStats() const {
    cout << "===== BANK =====\n";
    cout << "Loan Amount: " << loanAmount << " gold\n";
    cout << "Interest Rate: " << interestRate << "\n";
    cout << "Loan Status: " << (defaulted ? "Defaulted" : "Good") << "\n";
    cout << "Loan Term: " << loanTime << " turns\n";
}

ResourceManager::ResourceManager() {
    setFood(500);
    setWood(200);
    setStone(100);
    setGold(50);
}

void ResourceManager::distributeResources(Population* population) {
    int foodNeed = population->getTotalPopulation() / 2;

    if (getFood() >= foodNeed) {
        removeFood(foodNeed);
        cout << "Food distributed to population: " << foodNeed << "\n";
    }
    else {
        int starving = foodNeed - getFood();
        removeFood(getFood());

        int deaths = starving / 2;
        population->removePeasants(deaths);
        cout << "Food shortage! " << deaths << " peasants died from starvation.\n";
    }

    addFood(population->getPeasants() / 5);
    addWood(population->getPeasants() / 10);
    addStone(population->getPeasants() / 20);
    addGold(population->getMerchants() / 2);
}

bool ResourceManager::handleShortages(Population* population) {
    bool shortage = false;

    if (getFood() < population->getTotalPopulation() / 4) {
        cout << "WARNING Food shortage \n";
        shortage = true;
    }
    return shortage;
}

void ResourceManager::displayStats() const {
    cout << "===== RESOURCES =====\n";
    for (int i = 0; i < MAX_RESOURCES; i++) {
        cout << getResourceName(i) << ": " << getResource(i) << "\n";
    }
}

Market::Market() {
    itemName[0] = "Food";
    itemName[1] = "Wood";
    itemName[2] = "Stone";
    itemName[3] = "Gold";

    prices[0] = 1;  
    prices[1] = 2;  
    prices[2] = 5;  
    prices[3] = 10; 

    tradePrice = 0;
}

void Market::updatePrices() {
    for (int i = 0; i < MAX_MARKET_ITEMS; i++) {
        double change = (rand() % 21 - 10) / 100.0;
        prices[i] *= (1 + change);

        if (prices[i] < 0.1) prices[i] = 0.1;
    }
}

bool Market::trade(ResourceManager* resources, Economy* economy, int resourceIndex, int amount, bool buying) {
    if (resourceIndex < 0 || resourceIndex >= MAX_MARKET_ITEMS) {
        cout << "Invalid selection\n";
        return false;
    }

    double tradeAmount = amount * prices[resourceIndex];

    if (buying) {
        if (economy->removeFunds(tradeAmount)) {
            resources->addResource(resourceIndex, amount);
            tradePrice += amount;
            cout << "Bought " << amount << " " << itemName[resourceIndex]
                << " for " << tradeAmount << " gold.\n";
                return true;
        }
        else {
            cout << "Not enough gold in treasury!\n";
            return false;
        }
    }
    else {
        if (resources->removeResource(resourceIndex, amount)) {
            economy->addFunds(tradeAmount);
            tradePrice += amount;
            cout << "Sold " << amount << " " << itemName[resourceIndex]
                << " for " << tradeAmount << " gold.\n";
                return true;
        }
        else {
            cout << "Not enough " << itemName[resourceIndex] << " to sell!\n";
            return false;
        }
    }

    return false;
}

void Market::triggerSmuggling(ResourceManager* resources) {
    int resourceIndex = rand() % MAX_RESOURCES;
    int amount = resources->getResource(resourceIndex) / 10;

    if (amount > 0) {
        resources->removeResource(resourceIndex, amount);
        cout << " stolen "<< amount << " " << itemName[resourceIndex] << "!\n";
    }
}

void Market::displayStats() const {
    cout << "===== MARKET =====\n";
    cout << "Trading Volume: " << tradePrice << "\n";
    cout << "Current Prices:\n";

    for (int i = 0; i < MAX_MARKET_ITEMS; i++) {
        cout << "- " << itemName[i] << ": " << fixed << setprecision(2) << prices[i] << " gold\n";
    }
}

Politics::Politics() {
    stability = 50;
    coupAttempted = false;
    currentKingName = "None";
}

King* Politics::electKing(const string& name) {
    currentKingName = name;

    int leadershipScore = rand() % 10 + 1;

    cout << name << " has been elected as King with score " << leadershipScore << ".\n";
    return new King(name, leadershipScore);
}

void Politics::applyBribery(Economy* economy) {
    if (economy->removeFunds(200)) {
        stability += 10;
        cout << "Stability increased to " << stability << ".\n";
    }
    else {
        cout << "Not enough money for bribes\n";
    }
}

bool Politics::triggerCoup(Economy* economy, Population* population) {
    coupAttempted = true;

    int coupChance = 100 - stability;

    if (economy->getTreasury() < 500) {
        coupChance += 20;
    }

    if (population->getMilitary() < 50) {
        coupChance += 20;
    }

    bool coupSuccess = (rand() % 100) < coupChance;

    if (coupSuccess) {
        stability = 30;
        economy->setTreasury(economy->getTreasury() * 0.7); 
        cout << "COUP SUCCESSFUL" << endl;
return true;
    }
    else {
        cout << "Coup attempt failed.\n";
        stability -= 10;
        population->removeNobles(3);
        return false;
    }
}

void Politics::displayStats() const {
    cout << "===== POLITICS =====\n";
    cout << "Stability: " << stability << "%\n";
    cout << "Coup Attempted: " << (coupAttempted ? "Yes" : "No") << "\n";
    cout << "Current King: " << currentKingName << "\n";
}

EventSystem::EventSystem() {
    eventNames[0] = "Plague";
    eventNames[1] = "Market Crash";
    eventNames[2] = "Coup Attempt";
    eventNames[3] = "Smuggling";
    eventNames[4] = "Bountiful Harvest";

    eventPossibility[0] = 10;  
    eventPossibility[1] = 15;  
    eventPossibility[2] = 5;   
    eventPossibility[3] = 20;  
    eventPossibility[4] = 30;  
}

int EventSystem::randomEvent() {
    int roll = rand() % 100;
    int cumulativeProbability = 0;

    for (int i = 0; i < MAX_EVENTS; i++) {
        cumulativeProbability += eventPossibility[i];
        if (roll < cumulativeProbability) {
            return i;
        }
    }

    return -1; 
}

void EventSystem::handleEvent(int eventId, Kingdom* kingdom) {
    if (eventId < 0 || eventId >= MAX_EVENTS) {
        return;
    }

    displayEvent(eventId);

    switch (eventId) {
    case 0: 
        kingdom->getPopulation()->handlePlague();
        break;
    case 1: 
        kingdom->getEconomy()->triggerMarketCrash();
        break;
    case 2: 
        kingdom->getPolitics()->triggerCoup(kingdom->getEconomy(), kingdom->getPopulation());
        break;
    case 3: 
        kingdom->getMarket()->triggerSmuggling(kingdom->getResources());
        break;
    case 4: 
        int bonus = kingdom->getPopulation()->getPeasants() / 2;
        kingdom->getResources()->addFood(bonus);
        cout << "harvest! Added " << bonus << " food.\n";
        break;
    }
}

void EventSystem::displayEvent(int eventId) const {
    cout << "\n!!! RANDOM EVENT: " << eventNames[eventId] << " !!!\n";
}

bool GameIO::saveToFile(const string& filename, Kingdom* kingdom) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file \n";
        return false;
    }

    file << kingdom->getKingdomName() << "\n";
    file << kingdom->getTurnNumber() << "\n";
    file << kingdom->getScore() << "\n";

    Population* pop = kingdom->getPopulation();
    file << pop->getPeasants() << " " << pop->getMerchants() << " "
        << pop->getNobles() << " " << pop->getMilitary() << "\n";

    Army* army = kingdom->getArmy();
    file << army->getSoldiers() << " " << army->getMorale() << " "
        << army->isPaid() << " " << army->getWeaponQuality() << "\n";

    Economy* economy = kingdom->getEconomy();
    file << economy->getTreasury() << " " << economy->getTaxRate() << " "
        << economy->getInflationRate() << "\n";

    Bank* bank = kingdom->getBank();
    file << bank->getLoanAmount() << " " << bank->getInterestRate() << " "
        << bank->hasDefaulted() << " " << bank->getLoanTerm() << "\n";

    ResourceManager* res = kingdom->getResources();
    for (int i = 0; i < MAX_RESOURCES; i++) {
        file << res->getResource(i) << " ";
    }
    file << "\n";

    Politics* politics = kingdom->getPolitics();
    file << politics->getStability() << " " << politics->wasCoupAttempted() << " "
        << politics->getCurrentKingName() << "\n";

    file.close();
    cout << "Game saved successfully to " << filename << "\n";
    return true;
}

bool GameIO::loadFromFile(const string& filename, Kingdom* kingdom) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file for loading.\n";
        return false;
    }

    string kingdomName;
    int turnNumber, score;

    getline(file, kingdomName);
    file >> turnNumber >> score;
    file.ignore(); 

    int peasants, merchants, nobles, military;
    file >> peasants >> merchants >> nobles >> military;
    Population* pop = kingdom->getPopulation();
    pop->removePeasants(pop->getPeasants());
    pop->removeMerchants(pop->getMerchants());
    pop->removeNobles(pop->getNobles());
    pop->removeMilitary(pop->getMilitary());
    pop->addPeasants(peasants);
    pop->addMerchants(merchants);
    pop->addNobles(nobles);
    pop->addMilitary(military);

    int soldiers, morale, isPaid, weaponQuality;
    file >> soldiers >> morale >> isPaid >> weaponQuality;
    Army* army = kingdom->getArmy();
    army->setSoldiers(soldiers);
    army->setMorale(morale);
    army->setPaid(isPaid);
    while (army->getWeaponQuality() < weaponQuality) {
        army->improveWeapons();
    }

    double treasury, taxRate, inflationRate;
    file >> treasury >> taxRate >> inflationRate;
    Economy* economy = kingdom->getEconomy();
    economy->setTreasury(treasury);
    economy->setTaxRate(taxRate);

    double loanAmount, interestRate;
    bool defaulted;
    int loanTerm;
    file >> loanAmount >> interestRate >> defaulted >> loanTerm;
    Bank* bank = kingdom->getBank();
    bank->setLoanAmount(loanAmount);
    bank->setInterestRate(interestRate);
    bank->setDefaulted(defaulted);

    ResourceManager* res = kingdom->getResources();
    int resourceAmount;
    for (int i = 0; i < MAX_RESOURCES; i++) {
        file >> resourceAmount;
        res->setResource(i, resourceAmount);
    }

    int stability;
    bool coupAttempted;
    string kingName;
    file >> stability >> coupAttempted;
    file.ignore(); 
    getline(file, kingName);

    Politics* politics = kingdom->getPolitics();
    politics->setStability(stability);
    politics->setCoupAttempted(coupAttempted);
    politics->setCurrentKingName(kingName);

    int leadershipScore = 5;
    King* newKing = new King(kingName, leadershipScore);
    kingdom->setKing(newKing);

    file.close();
    cout << "Game loaded successfully from " << filename << "\n";
    return true;
}

void GameIO::saveScore(const string& playerName, int score) {
    ofstream file("highscores.txt", ios::app);
    if (file.is_open()) {
        file << playerName << " " << score << "\n";
        file.close();
        cout << "Score saved!\n";
    }
}

void GameIO::displayHighScores() const {
    ifstream file("highscores.txt");

    cout << "===== HIGH SCORES =====\n";

    if (!file.is_open()) {
        cout << "No high scores\n";
        return;
    }

    string name;
    int score;

    while (file >> name >> score) {
        cout << name << ": " << score << "\n";
    }

    file.close();
}

Kingdom::Kingdom(const string& name) : kingdomName(name), turnNumber(1), score(0) {
    population = new Population();
    army = new Army();
    economy = new Economy();
    politics = new Politics();
    bank = new Bank();
    market = new Market();
    resources = new ResourceManager();
    events = new EventSystem();
    gameIO = new GameIO();

    king = nullptr;
    commander = new Commander("None", 5);
    guildLeader = new GuildLeader("None", 5);

    srand(static_cast<unsigned int>(time(nullptr)));
}

Kingdom::~Kingdom() {
    delete population;
    delete army;
    delete economy;
    delete politics;
    delete bank;
    delete market;
    delete resources;
    delete events;
    delete gameIO;
    delete king;
    delete commander;
    delete guildLeader;
}

void Kingdom::runTurn() {
    turnNumber++;

    population->updateGrowth();
    army->updateMorale();
    market->updatePrices();
    resources->distributeResources(population);
    bank->audit();

    if (king) king->policyEffect();
    commander->policyEffect();
    guildLeader->policyEffect();

    int eventId = events->randomEvent();
    if (eventId >= 0) {
        events->handleEvent(eventId, this);
    }

    score = population->getTotalPopulation() + army->getSoldiers() * 2 +
        static_cast<int>(economy->getTreasury() / 10) + politics->getStability();

    cout << "\nTurn " << turnNumber << " completed.\n";
    cout << "Current score: " << score << "\n";
}

void Kingdom::displayStats() const {
    printStrongholdBanner();

    cout << "\n===== KINGDOM OF " << kingdomName << " =====\n";
    cout << "Turn: " << turnNumber << "\n";
    cout << "Score: " << score << "\n\n";

    population->displayStats();
    cout << "\n";

    army->displayStats();
    cout << "\n";

    economy->displayStats();
    cout << "\n";

    bank->displayStats();
    cout << "\n";

    politics->displayStats();
    cout << "\n";

    resources->displayStats();
    cout << "\n";

    market->displayStats();
    cout << "\n";

    cout << "===== LEADERS =====\n";
    if (king) {
        cout << "King: " << king->getName() << " (Leadership: " << king->getLeadershipScore() << ")\n";
    }
    else {
        cout << "No king\n";
    }
    cout << "Commander: " << commander->getName() << " (Leadership: " << commander->getLeadershipScore() << ")\n";
    cout << "Guild Leader: " << guildLeader->getName() << " (Leadership: " << guildLeader->getLeadershipScore() << ")\n";
}

bool Kingdom::saveGame(const string& filename) {
    return gameIO->saveToFile(filename, this);
}

bool Kingdom::loadGame(const string& filename) {
    return gameIO->loadFromFile(filename, this);
}

void Kingdom::gameOver() {
    cout << "\n===== GAME OVER =====\n";
    cout << "Kingdom: " << kingdomName << "\n";
    cout << "Duration : " << turnNumber << " turns\n";
    cout << "Score: " << score << "\n";

    string playerName;
    cout << "Enter your name for high score: ";
    cin >> playerName;

    gameIO->saveScore(playerName, score);
    gameIO->displayHighScores();
}

void waitKey() {
    cout << "\nPress Enter to continue...";

    cin.get();
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printStrongholdBanner() {
    cout << "                            ===============================================\n";
    cout << "                              ____  _                       _           \n";
    cout << "                             / ___|| |_ _ __ ___  _ __  |_____||\n";
    cout << "                             /___ /| __| '__/ _ //| '_ / | | | |\n";
    cout << "                              ___) | |_| | | (_) | | | | |_| |\n";
    cout << "                             |____/|___|_|  /___/|_| |_|/|///\n";
    cout << "                                                         |___/             \n";
    cout << "                            ===============================================\n";
    cout << "                                     Medieval Kingdom Simulator           \n";
    cout << "                            ===============================================\n\n";
}

void displayMenu() {

    cout << "MAIN MENU\n";
    cout << "1. New Game\n";
    cout << "2. Load Game\n";
    cout << "3. View High Scores\n";
    cout << "4. Quit\n";
    cout << "Enter your choice: ";
}

void displayGameMenu() {
    printStrongholdBanner();

    cout << "GAME MENU\n";
    cout << "==========\n\n";
    cout << "1. Display Kingdom Status\n";
    cout << "2. Trade Menu\n";
    cout << "3. Military Menu\n";
    cout << "4. Financial Menu\n";
    cout << "5. End Turn\n";
    cout << "6. Save Game\n";
    cout << "7. Return to Main Menu\n";
    cout << "==========\n\n";

    cout << "Enter your choice: ";
    cout << "==================";

}

void displayTradeMenu(Kingdom* kingdom) {
    clearScreen();
    cout << "===== TRADE MENU =====\n";
    cout << "Treasury: " << kingdom->getEconomy()->getTreasury() << " gold\n\n";

    cout << "Available Resources:\n";
    for (int i = 0; i < MAX_RESOURCES; i++) {
        cout << i + 1 << ". " << kingdom->getResources()->getResourceName(i)
            << ": " << kingdom->getResources()->getResource(i)
            << " (Market Price: " << kingdom->getMarket()->getPrice(i) << " gold)\n";
    }

    cout << "\n1. Buy Resources\n";
    cout << "2. Sell Resources\n";
    cout << "3. Back to Game Menu\n";
    cout << "Enter your choice: ";
}

void displayMilitaryMenu(Kingdom* kingdom) {
    clearScreen();
    kingdom->getArmy()->displayStats();
    cout << "\n===== MILITARY MENU =====\n";
    cout << "1. Train Troops\n";
    cout << "2. Improve Weapons\n";
    cout << "3. Go to War\n";
    cout << "4. Back to Game Menu\n";
    cout << "Enter your choice: ";
}

void displayFinancialMenu(Kingdom* kingdom) {
    clearScreen();
    kingdom->getEconomy()->displayStats();
    kingdom->getBank()->displayStats();

    cout << "\n===== FINANCIAL MENU =====\n";
    cout << "1. Collect Taxes\n";
    cout << "2. Pay Military Wages\n";
    cout << "3. Take Loan\n";
    cout << "4. Repay Loan\n";
    cout << "5. Back to Game Menu\n";
    cout << "Enter your choice: ";
}

void handleTradeMenu(Kingdom* kingdom) {
    int choice, resourceChoice, amount;

    do {
        displayTradeMenu(kingdom);
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Which resource (1-" << MAX_RESOURCES << "): ";
            cin >> resourceChoice;

            if (resourceChoice < 1 || resourceChoice > MAX_RESOURCES) {
                cout << "Invalid selection.\n";
                break;
            }

            cout << "How much to buy: ";
            cin >> amount;

            kingdom->getMarket()->trade(kingdom->getResources(), kingdom->getEconomy(),
                resourceChoice - 1, amount, true);
            waitKey();
            break;

        case 2: 
            cout << "Which resource (1-" << MAX_RESOURCES << "): ";
            cin >> resourceChoice;

            if (resourceChoice < 1 || resourceChoice > MAX_RESOURCES) {
                cout << "Invalid selection.\n";
                break;
            }

            cout << "How much to sell: ";
            cin >> amount;

            kingdom->getMarket()->trade(kingdom->getResources(), kingdom->getEconomy(),
                resourceChoice - 1, amount, false);
            waitKey();
            break;

        case 3: 
            return;

        default:
            cout << "Invalid choice\n";
            break;
        }
    } while (true);
}

void handleMilitaryMenu(Kingdom* kingdom) {
    int choice;

    do {
        displayMilitaryMenu(kingdom);
        cin >> choice;

        switch (choice) {
        case 1: 
            kingdom->getArmy()->trainTroops();
            waitKey();
            break;

        case 2: 
            if (kingdom->getEconomy()->removeFunds(100)) {
                kingdom->getArmy()->improveWeapons();
                cout << "Weapons improved! Quality now: " << kingdom->getArmy()->getWeaponQuality() << "\n";
            }
            else {
                cout << "Not enough gold to improve weapons.\n";
            }
            waitKey();
            break;

        case 3: 
        {
            int casualties = kingdom->getArmy()->goToWar();
            kingdom->getPopulation()->adjustPopulationAfterWar(casualties);
        }
        waitKey();
        break;

        case 4: 
            return;

        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (true);
}

void handleFinancialMenu(Kingdom* kingdom) {
    int choice;
    double amount;

    do {
        displayFinancialMenu(kingdom);
        cin >> choice;

        switch (choice) {
        case 1: 
            kingdom->getEconomy()->collectTaxes(kingdom->getPopulation(), kingdom->getKing());
            waitKey();
            break;

        case 2: 
            kingdom->getEconomy()->payWages(kingdom->getPopulation(), kingdom->getArmy());
            waitKey();
            break;

        case 3:
            cout << "Enter loan amount: ";
            cin >> amount;
            kingdom->getBank()->giveLoan(kingdom->getEconomy(), amount);
            waitKey();
            break;

        case 4: 
            kingdom->getBank()->repayLoan(kingdom->getEconomy());
            waitKey();
            break;

        case 5: 
            return;

        default:
            cout << "Invalid choice\n";
            break;
        }
    } while (true);
}

void playGame(Kingdom* kingdom) {
    int choice;
    bool gameRunning = true;

    
    string kingName;
    cout << "Enter a name for your king: ";
    cin.ignore();
    getline(cin, kingName);

    kingdom->setKing(kingdom->getPolitics()->electKing(kingName));

    while (gameRunning) {
        clearScreen();
        displayGameMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            clearScreen();
            kingdom->displayStats();
            waitKey();
            break;

        case 2: 
            printStrongholdBanner();

            handleTradeMenu(kingdom);
            break;

        case 3:
            printStrongholdBanner();

            handleMilitaryMenu(kingdom);
            break;

        case 4: 
            printStrongholdBanner();

            handleFinancialMenu(kingdom);
            break;

        case 5: 
            kingdom->runTurn();
            waitKey();

            
            if (kingdom->getPopulation()->getTotalPopulation() <= 10 ||
                kingdom->getArmy()->getSoldiers() <= 0) {
                cout << "Your kingdom has fallen!\n";
                kingdom->gameOver();
                gameRunning = false;
            }
            break;

        case 6:
        {
            string filename;
            cout << "Enter save file name: ";
            cin >> filename;
            kingdom->saveGame(filename + ".sav");
            waitKey();
        }
        break;

        case 7: 
            gameRunning = false;
            break;

        default:
            cout << "Invalid choice\n";
            break;
        }
    }
}