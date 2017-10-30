#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
using namespace std;

//this structure is used by the other 3 structures as a way of "defining" weapon attributes
struct weaponstats{
    string name; //weapon name
    int accuracy; //weapon accuracy %
    int might; // weapon power
    int critical; //weapon critical hit rate
};

//This structure is used to give the Player, enemy and boss a list of unique skills that can turn the tide of battle!
struct skills{
    string skillname;
    int likelyhood;
    string description;
};

//this structure will allow the player, enemy and boss to define their combat stats by assigning them to a unique variable
//rather than assigning them to a member array, which will allow be to use each stat as an identifier rather than an array subscript
struct unitstats{
	int attack;//how hard a unit hits
    int skill;//determines accurate a unit is
    int speed;//increases the avoid stat, increasing chances of dodging
    int luck;//increases chances of dodging, landing attacks, and reduces chances of getting crit'd
    int defense;//lowers damage taken
    int avoid;//chances of dodging attacking. The higher this stat it is, the harder it is to hit this unit
    int hitrate;//hit % after calculating skill and luck
    int effectiveHitrate;//hitrate - avoid. hitrate after taking into account opponents avoid stat
    int critrate;//crit % after calculating skill
    int effectiveCritRate;//crit rate after taking into account opponents luck stat
    int totalAttack;//units attack stat plus the might of their weapon
};

//this structure will define the player, and allow the program to input the users stats, weapons and skills
struct createplayer{
    string name;
    int HP;
    int level = 0;
    unitstats stats;//the stats of the player
    weaponstats weapon;//Here is the players weapon
    skills playerskill;//Finally, here is the player's skill.
    int arrayofstats[5];
};
//this structure functions very similarly to createplayer, but will instead be used to create an array of bandits for the play
//to fight against
struct createenemy{
    string name;
    int enemyHP;
    unitstats stats;
    weaponstats weapon;
};
//creates a boss for the player to fight after killing all of the bandits
struct createboss{
    string name;
    int HP;
    unitstats stats;
    weaponstats weapon;
    skills boss_skill;
};

void playGame(createplayer player, createenemy enemy[], createboss boss);
void skillmenu();
void printskill(const createplayer player);
void generateBoss(createboss& boss);
void adjustBattleStats(createplayer &player, createenemy enemy[], int numenemies);
void generateEnemy(createenemy enemy[]);
void readplayer(createplayer &player);
void upgradeWeapon(createplayer player);
void lunaAgainstBandit(int numenemies, createplayer player, createenemy enemy[]);
void astraAgainstBandit(int numenemies, createplayer player, createenemy enemy[]);
void lunaAgainstBoss(createplayer player, createboss boss);
void astraAgainstboss(createplayer player, createboss boss);
void enemycritical(int numenemies, createenemy enemy[], createplayer player);
void enemyretaliate(int numenemies, createenemy enemy[], createplayer player);
void bossretaliate(createboss boss, createplayer player);
void bosscritical(createboss boss, createplayer player);
void boss_skillactivate(createboss boss, createplayer player);
bool fightAgainstBandit(createplayer player, int numenemies, createenemy enemy[]);
bool bossSequence(int numenemies, createboss boss, createplayer player);
void heal(int numenemies, string signal, int &numheal, createenemy enemy[], createplayer player, createboss boss);
void choice();
void levelup(createplayer &player);
void makeStronger(int numenemies, createenemy enemy[]);
void setPlayerStats(createplayer &player);
void printweapon(const createplayer player);
void gameIntroduction(createplayer &player);
void printstats(const createplayer player);
void printenemystats(const createenemy enemy[]);
void criticalAgainstBoss(createplayer player, createboss boss);
void criticalAgainstBandit(int numenemies, createplayer player, createenemy enemy[]);

int main()
{
    createplayer player;
    createenemy enemy[5];
    createboss boss;
    ifstream infile ("enemynames.txt");
    int num = 0;
    string description[5]{"Attack", "Skill", "Speed", "Luck", "Defense"};

    infile >> enemy[num].name;
    while (infile){
        num++;
        infile >> enemy[num].name;
    }

    generateEnemy(enemy);
    gameIntroduction(player);
    adjustBattleStats(player, enemy, num);
    printenemystats(enemy);
    cout << endl;
    printstats(player);
    //playGame(player, enemy, boss);

    return 0;
}

void gameIntroduction(createplayer &player)
{
    cout << "Welcome to the land of...Tellius! In order to test your abilities, You will fight off three bandits" << endl;
    cout << "attacking a village. Will you live? will you die? Let's see :)" << endl << endl;
    cout << "In order to become accustomed to this new location, you will allowed to customize your character! " << endl;
    readplayer(player);
    cout << "Now that you have been established, you need to hurry quickly! A village is being attacked!" << endl;
    cout << "The bandit attacking turned his eyes on you...what do you do? " << endl;
}
//this function is used to "create" the player character. I used create lightly because your stats and skills are
//randomized, rather than player controlled. I did this for simplicity!
void readplayer(createplayer &player)
{
    int randomskill;
    string description[5]{"Attack", "Skill", "Speed", "Luck", "Defense"};

    cout << "What is your name?: ";
    getline(cin, player.name);
    cout << "Greetings " << player.name << "!" << endl << endl;

    cout << "Here are your starting stats!" << endl;
    player.HP = 150;
    cout << "HP: " << player.HP << endl;
    setPlayerStats(player);
    printstats(player);

    srand(time(0));
    randomskill = rand() % 2 + 1;

    if (randomskill == 1){
        player.playerskill.skillname = "Luna";
        player.playerskill.description = "Negate enemy defense when attacking!";
        player.playerskill.likelyhood = player.stats.skill * 1.5; //Luna has a skill * 1.5 percent chance of activating
        printskill(player);
    }else{
        player.playerskill.skillname = "Astra";
        player.playerskill.description = "Attack Five times in a row at half damage!";
        player.playerskill.likelyhood = player.stats.skill;//Astra has a skill % chance of activating
        printskill(player);
    }
}

void printstats(const createplayer player)
{
    cout << "Attack: " << player.stats.attack << endl;
    cout << "Skill: " << player.stats.skill << endl;
    cout << "Speed: " << player.stats.speed << endl;
    cout << "Luck: " << player.stats.luck << endl;
    cout << "Defense: " << player.stats.defense << endl;
    cout << "Avoid: " << player.stats.avoid << endl;
    cout << "Total attack: " << player.stats.totalAttack << endl;
    cout << "Hit rate: " << player.stats.hitrate << endl;
    cout << "crit rate: " << player.stats.critrate << endl;
    cout << "effective hit rate: " << player.stats.effectiveHitrate << endl;
    cout << "effective crit rate: " << player.stats.effectiveCritRate << endl;
}

void printskill(createplayer player)
{
    cout << "Your first skill is " << player.playerskill.skillname << "!" << endl;
    cout << "Skill description: " << player.playerskill.description << endl;
    cout << "Activation chance: " << player.playerskill.likelyhood << "%" << endl << endl;
}

void printenemystats(createenemy enemy[])
{
    for(int i = 0; i < 5; i++){
        cout << enemy[i].name << " stats are: " << endl;
        cout << "Attack: " << enemy[i].stats.attack << endl;
        cout << "Skill: " << enemy[i].stats.skill << endl;
        cout << "Speed: " << enemy[i].stats.speed << endl;
        cout << "Luck: " << enemy[i].stats.luck << endl;
        cout << "Defense: " << enemy[i].stats.defense << endl;
        cout << "total attack: " << enemy[i].stats.totalAttack << endl;
        cout << "effective hit rate: " << enemy[i].stats.effectiveHitrate << endl;
        cout << "effective crit rate: " << enemy[i].stats.effectiveHitrate << endl;
        cout << "avoid: " << enemy[i].stats.avoid << endl;
    }
}

void setPlayerStats(createplayer &player)
{
    player.weapon.name = "Silver Sword";
    player.weapon.accuracy = 95;
    player.weapon.might = 14;
    player.weapon.critical = 20;

    srand(time(0));
    player.HP = 150;
    player.stats.attack = rand() % 5 + 20;
    player.stats.skill = rand() % 5 + 20;
    player.stats.speed = rand() % 5 + 20;
    player.stats.luck = rand() % 5 + 20;
    player.stats.defense = rand() % 5 + 20;
    player.stats.avoid = (player.stats.speed * 2) + player.stats.luck;
    player.stats.totalAttack = player.stats.attack + player.weapon.might;
    player.stats.hitrate = (player.stats.skill * 2) + player.weapon.accuracy + player.stats.luck;
    player.stats.critrate = player.stats.skill + player.weapon.critical;
}

void adjustBattleStats(createplayer &player, createenemy enemy[], int numenemies)
{
    int temporaryLuck = player.stats.luck, temporaryAvoid = player.stats.avoid;

    player.stats.effectiveCritRate = (player.stats.critrate - enemy[numenemies].stats.luck);
    player.stats.effectiveHitrate = (player.stats.hitrate - enemy[numenemies].stats.avoid);
    enemy[numenemies].stats.effectiveCritRate = (enemy[numenemies].stats.critrate - temporaryLuck);
    enemy[numenemies].stats.effectiveHitrate = (enemy[numenemies].stats.hitrate - temporaryAvoid);
    cout << "player stats: " << endl;
    cout << "effective hit rate: " << player.stats.effectiveHitrate << endl;
    cout << "effective crit rate: " << player.stats.effectiveCritRate << endl << endl;
    cout << "enemy stats: " << endl;
    cout << "effective hit rate: " << enemy[numenemies].stats.effectiveHitrate << endl;
    cout << "effective crit rate: " << enemy[numenemies].stats.effectiveHitrate << endl;
}

void playGame(createplayer player, createenemy enemy[], createboss boss)
{
    int numenemies = 0, selection, numheal = 10;
    string signal = "enemy";
    bool continueGame = true;


    adjustBattleStats(player, enemy, numenemies);//adjust the effective stats, which require both the createplayer and createenemy structs
    while(numenemies < 5){ //to fall out of outer loop, you must kill five bandits
        while(continueGame){ //in order to fall out of inner loop, battle function must return false, signifying a bandit was killed
           // printenemystats(enemy);
            choice();
            cin >> selection;
            while (selection != 1 && selection != 2){ // while loop used to ensure selection has either a 1 or 2
                cout << selection << " is not valid. Try again: ";
                choice();
                cin >> selection;
            }
            if(selection == 1)
                continueGame = fightAgainstBandit(player, numenemies, enemy);
            else if (selection == 2 && numheal > 0)
                heal(numenemies, signal, numheal, enemy, player, boss);
            else
                cout << "No more vulneraries!" << endl;

            if(player.HP <= 0){
            	cout<< "Try again? y to continue, n to quit: ";
            	char again;
            	cin >> again;
            	if(again == 'y' || again == 'Y'){
                    continueGame = true;
            		player.HP = 150;
            		enemy[numenemies].enemyHP = 150;
            		numheal = 10;
				}
				else{
					cout << "Aww too bad :( Thank you for playing!" << endl;
					exit(0);
				}
			}
        }
        continueGame = true; //loop is set to true to reset inner loop
        levelup(player);//after a bandit is killed, player levels up!
        numenemies++;//increment numenemies so it can be used a subscript in the array of enemies
        player.HP = 150;
        makeStronger(numenemies, enemy);//makes the next enemy stronger than the last
        adjustBattleStats(player, enemy, numenemies);
        cout << enemy[numenemies].name << " is attacking you now!" << endl;
    }

}

void printweapon(createplayer player)
{
    cout << "weapon might: " << player.weapon.might << endl;
    cout << "weapon name: " << player.weapon.name << endl;
    cout << "weapon accuracy: " << player.weapon.accuracy << endl;
    cout << "weapon critical: " << player.weapon.critical << endl;
}

void buffenemystats(int buffAmount, createenemy enemy[], int numenemies)
{
    enemy[numenemies].stats.attack += buffAmount;
    enemy[numenemies].stats.skill += buffAmount;
    enemy[numenemies].stats.speed += buffAmount;
    enemy[numenemies].stats.luck += buffAmount;
    enemy[numenemies].stats.defense += buffAmount;
    enemy[numenemies].weapon.might += buffAmount;
}

//After a bandit is killed, the next one will be even stronger than the last one
void makeStronger(int numenemies, createenemy enemy[])
{
    if (numenemies == 1)
        buffenemystats(3, enemy, numenemies);
    else if (numenemies == 2)
        buffenemystats(5, enemy, numenemies);
    else if (numenemies == 3)
        buffenemystats(7, enemy, numenemies);
    else{
        buffenemystats(12, enemy, numenemies);
        enemy[numenemies].weapon.critical += 10;
        enemy[numenemies].weapon.accuracy = 90;
    }
}

//After the player attacks the boss, he will attack back! However, a function is needed due this process taking various steps
//can you imagine how short this program would have been had I not calculated critical rate, accuracy OR skills?
void bossretaliate (createboss boss, createplayer player)
{
	int bosscritchance, damage,  skillproc;

	damage = boss.stats.totalAttack - player.stats.defense;

	srand(time(NULL));
	bosscritchance = rand () % 100;
	skillproc = rand () % 100;

    cout << boss.name << " retaliated!" << endl;

    if (bosscritchance < boss.stats.critrate) //determines whether or not boss lands a critical hit
        bosscritical(boss, player);
    else if(skillproc < boss.boss_skill.likelyhood)
    	boss_skillactivate(boss, player);
   	else{
		cout << "You took " << damage << " damage!" << endl;
        player.HP -= damage;
        cout << "You have  " << player.HP << " HP left!" << endl << endl;
	}
}

//Does the same thing as the above function, but for the enemy instead of the boss
void enemyretaliate(int numenemies, createenemy enemy[], createplayer player)
{
	int critchance, rn, damage;

	damage = enemy[numenemies].stats.totalAttack - player.stats.defense;
	srand(time(NULL));
	critchance = rand () % 100;

    cout << enemy[numenemies].name << " retaliated!" << endl;
	if(critchance < enemy[numenemies].stats.effectiveCritRate)
        enemycritical(numenemies, enemy, player);
    else{
        cout << "You took " << damage << " damage!" << endl;
        player.HP -= damage;
        cout << "You have  " << player.HP << " HP left!" << endl << endl;
    }
}

void choice()
{
    cout << "Attack or heal?" << endl;
    cout << "1 = Attack!" << endl;
    cout << "2 = Heal!" << endl;
}

//After the player kills a bandit, player levels up and gets increased stats
void levelup(createplayer &player)
{
    string description[5]{"Attack", "Skill", "Speed", "Luck", "Defense"};

    cout << "You leveled up! Level is " << ++player.level << endl;

    player.stats.attack += 3;
    player.stats.skill += 3;
    player.stats.speed += 3;
    player.stats.luck += 3;
    player.stats.defense += 3;

    printstats(player);
}

//This function allows the player to heal HP using a healing item. The remaining amount is then returned to main
//NOTE: If you heal, you will get attacked, as sen in the below code
void heal (int numenemies, string signal, int &numheal, createenemy enemy[], createplayer player, createboss boss)
{
    player.HP += 30;
    numheal--;
    cout << "You have " << numheal << " vulneraries left!" << endl;
    cout << "HP healed by 30! Current HP: " << player.HP << endl << endl;

    //you don't get to heal for free! If you heal, you still get attacked, so be careful!
    if (signal == "enemy")
        enemyretaliate(numenemies, enemy, player);
    else
        bossretaliate(boss, player);
}

//The biggest function in the program, this function is in charge of controlling the flow of the battle between the bandits
//and the player. SEVERAL variables are declared, which serve to make function clearer as to what I am doing the player stats
//and enemy stats.
bool fightAgainstBandit(createplayer player, int numenemies, createenemy enemy[])
{
    int hitchance, skillchance, critchance, ecritchance, rn, damage;

    srand(time(NULL));
    hitchance = rand() % 100;
    skillchance = rand() % 100;
    critchance = rand() % 100;
    ecritchance = rand() % 100;
    rn = rand() % 100;

    if(hitchance < player.stats.effectiveHitrate){ //Since rn is less than hit, attack will connect
        if (skillchance < player.playerskill.likelyhood && player.playerskill.skillname == "Astra")
            astraAgainstBandit(numenemies, player, enemy);//activate astra
        else if (skillchance < player.playerskill.likelyhood && player.playerskill.skillname == "Luna")
            lunaAgainstBandit(numenemies, player, enemy);//activate luna
        else if(critchance < player.stats.effectiveCritRate) //player lands a critical
            criticalAgainstBandit(numenemies, player, enemy);
        else{ //regular hit
            damage = player.stats.totalAttack - enemy[numenemies].stats.defense;
            cout << "You landed an attack! " << enemy[numenemies].name << " took " << damage << " damage!" << endl;
            enemy[numenemies].enemyHP -= damage;
            cout << enemy[numenemies].name << " has " << enemy[numenemies].enemyHP << " HP left!" << endl << endl;
        }

        //if after attacking the bandit his hp is reduced to zero, return to main function
        if(enemy[numenemies].enemyHP <= 0){
	        cout << enemy[numenemies].name << " is dead!" << endl << endl;
	        return false;
        }

        if(rn < enemy[numenemies].stats.effectiveHitrate)
        	enemyretaliate(numenemies, enemy, player);
        else
            cout << enemy[numenemies].name << " missed his attack!" << endl;

    }else{ // attack misses
        cout << "You missed your attack!" << endl;
        if(rn < enemy[numenemies].stats.effectiveHitrate)
            enemyretaliate(numenemies, enemy, player);
        else
            cout << enemy[numenemies].name << " missed his attack!" << endl;
    }

    return true; //if neither the player's or enemy's HP is reduced to zero, return true, and continue the game loop in main
}

//This function defines the skill "Luna" and does damage according to its effect
void lunaAgainstBandit(int numenemies, createplayer player, createenemy enemy[])
{
    int critrn;

    srand(time(NULL));
    critrn = rand() % 100; //"rn" stands for random number!

    if (critrn < player.stats.critrate){
        cout << "You activated Luna AND landed a critical!" << endl;
        enemy[numenemies].enemyHP -= player.stats.totalAttack * 3;// Luna ignores critical, so enemy defense is not calculated!
        cout << enemy[numenemies].name << " took " << player.stats.totalAttack * 3 << " damage!" << endl;
        cout << enemy[numenemies].name << " has " << enemy[numenemies].enemyHP << " remaining" << endl;
    }else{
       cout << "You activated Luna!" << endl;
       enemy[numenemies].enemyHP -= player.stats.totalAttack;
       cout << enemy[numenemies].name << " took " << player.stats.totalAttack << " damage!" << endl;
       cout << enemy[numenemies].name << " has " << enemy[numenemies].enemyHP << " remaining" << endl;
    }
}

void lunaAgainstboss(createplayer player, createboss boss)
{
    int critrn;

    srand(time(NULL));
    critrn = rand() % 100; //"rn" stands for random number!

    if (critrn < player.stats.critrate){//if signal is "enemy", attack enemy. otherwise, attack boss
        cout << "You activated Luna AND landed a critical!" << endl;
        boss.HP -= player.stats.totalAttack * 3;// Luna ignores critical, so enemy defense is not calculated!
        cout << boss.name << " took " << player.stats.totalAttack * 3 << " damage!" << endl;
        cout << boss.name << " has " << boss.HP << " remaining" << endl;
    }
    else{
       cout << "You activated Luna!" << endl;
        boss.HP -= player.stats.totalAttack;// Luna ignores critical, so enemy defense is not calculated!
        cout << boss.name << " took " << player.stats.totalAttack << " damage!" << endl;
        cout << boss.name << " has " << boss.HP << " remaining" << endl;
    }
}

//This function defines the skill "Astra" and does damage according to its effect
void astraAgainstBandit(int numenemies, createplayer player, createenemy enemy[])
{
    int critrn, damage, rn;

    srand(time(NULL));

    damage = player.stats.totalAttack - enemy[numenemies].stats.defense;
    cout << "You activated Astra!" << endl;
    for(int i = 0; i < 5; i++){ //astra attacks five times, so for loop is perfect for it
        critrn = rand () % 100;
        rn = rand () % 100;

        if (rn < player.stats.effectiveHitrate){
            if (critrn < player.stats.effectiveCritRate){
                cout << "Critical hit!"  << (damage/2) * 3 << " dealt!" << endl;
                enemy[numenemies].enemyHP -= (damage/2) * 3;
            }
            else{
                cout << "You attacked! " << damage/2 << " dealt!" << endl;
                enemy[numenemies].enemyHP -= damage/2;
            }
        }else
            cout << "Your attack missed" << endl;
    }
     cout << enemy[numenemies].name << " has " << enemy[numenemies].enemyHP << " left!" << endl;
}

void astraAgainstBoss(createplayer player, createboss boss)
{
    int critrn, damage, rn;

    srand(time(NULL));

    damage = player.stats.totalAttack - boss.stats.defense;
    cout << "You activated Astra!" << endl;
    for(int i = 0; i < 5; i++){ //astra attacks five times, so for loop is perfect for it
        critrn = rand () % 100;
        rn = rand () % 100;

        if (rn < player.stats.effectiveHitrate){
            if (critrn < player.stats.effectiveCritRate){
                cout << "Critical hit!"  << (damage/2) * 3 << " dealt!" << endl;
                boss.HP -= (damage/2) * 3;
            }
            else{
                cout << "You attacked! " << damage/2 << " dealt!" << endl;
                boss.HP -= damage/2;
            }
        }else
            cout << "Your attack missed" << endl;
    }
    cout << boss.name << " has " << boss.HP << " left!" << endl;
}

//Similar to battleSequence, but it uses the boss's stats instead of the Bandit's.
bool bossSequence(int numenemies, createboss boss, createplayer player)
{
    int hitchance, skillchance, criticalchance, ecrit, rn, damage;

    srand(time(NULL));

    hitchance = rand() % 100;
    skillchance = rand() % 100;
    criticalchance = rand() % 100;
    ecrit = rand() % 100;
    rn = rand() % 100;

	//hitchance is a random number that will be used to determine whether or not your attack will connect. if hit chance is less
	//than battle hit, your attack will land. Otherwise, it will miss.
    if(hitchance < player.stats.effectiveHitrate){
        if (skillchance < player.playerskill.likelyhood && player.playerskill.skillname == "Astra")
            astraAgainstBoss(player, boss);
        else if(skillchance < player.playerskill.likelyhood && player.playerskill.skillname == "Luna")
            lunaAgainstboss(player, boss);
        else if(criticalchance < player.stats.effectiveCritRate)
            criticalAgainstBoss(player, boss);
        else{
            damage = (player.stats.totalAttack - boss.stats.defense);
            cout << "You landed an attack! " << boss.name << " took " << damage << " damage!" << endl;
            boss.HP -= damage;
            cout << boss.name << " has " << boss.HP << " HP left!" << endl;
        }

        //if after attacking the boss his hp is reduced to zero, you win the game!
        if(boss.HP <= 0){
            cout << "You killed " << boss.name << "! Congratulations!" << endl;
            cout << "You saved the town and rescued everybody! THANKS FOR PLAYING!!" << endl;
            exit(0);
        }
        if(rn < boss.stats.effectiveHitrate){ // call the funciton to let boss retaliate against player
            cout << boss.name << " retaliated! " << endl;
           	bossretaliate(boss, player);
    	}
        else
            cout << boss.name << " missed his attack!" << endl;

     }
     else{// attack misses
        cout << "You missed your attack!" << endl;
        if(rn < boss.stats.effectiveHitrate)
            bossretaliate(boss, player);
        else
            cout << boss.name << " missed his attack!" << endl;
    }

    return true; //return true to main function if neither player or boss is killed
}

//Function that allows boss to land critical hit
void bosscritical(createboss boss, createplayer player)
{
    int damage;

    damage = (player.stats.totalAttack - player.stats.defense) * 3;
    cout << boss.name << " landed a critical! You took " << damage << " damage!" << endl;
    player.HP -= damage;
    cout << player.name << ", You have " << player.HP << " HP left!" << endl;
}

//Boss can now use skills in combat
void boss_skillactivate(createboss boss, createplayer player)
{
    int crit, damage;

    srand(time(NULL));
    crit = rand() % 100;

	//a random number is generated, and than a comparison is made. If the random number is LESS than than value
	//of the effectiveCritrate, boss will than activate his skill "Luna" AND deal a critical hit. Otherwise, just activate skill
    if (crit < boss.stats.effectiveCritRate){
        cout << boss.name << " activated Luna AND landed a critical!" << endl;
        damage = boss.stats.totalAttack * 3;
        player.HP -= damage;
        cout << player.name << ", You took " << damage << " damage!" << endl;
        cout << player.name << ", You have " << player.HP << " remaining" << endl;
    }
    else{
        cout << boss.name << " activated Luna!" << endl;
        player.HP -= boss.stats.totalAttack;
        cout << player.name << "You took " << boss.stats.totalAttack << " damage!" << endl;
        cout << player.name << "You have " << player.HP << " remaining" << endl;
    }
}

//Player lands a critical
void criticalAgainstBandit(int numenemies, createplayer player, createenemy enemy[])
{
    int damage;

    damage = (player.stats.totalAttack - enemy[numenemies].stats.defense) * 3; //crit does 3 times normal damage!
    cout << "You landed a critical! " << enemy[numenemies].name << " took " << damage << " damage!" << endl;
    enemy[numenemies].enemyHP -= damage;
    cout << enemy[numenemies].name << " has " << enemy[numenemies].enemyHP << " HP left!" << endl;
}

void criticalAgainstBoss(createplayer player, createboss boss)
{
    int damage;

    damage = (player.stats.totalAttack - boss.stats.defense) * 3;
    cout << "You landed a critical! " << boss.name << " took " << damage << " damage!" << endl;
    boss.HP -= damage;
    cout << boss.name << " has " << boss.HP << " HP left!" << endl;
}

void enemycritical(int numenemies, createenemy enemy[], createplayer player)
{
    int damage;

    damage = (enemy[numenemies].stats.totalAttack - player.stats.defense) * 3; //crit does 3 times normal damage!
    cout << enemy[numenemies].name << " landed a critical! you took " << damage << " damage!" << endl;
    player.HP -= damage;
    cout  << player.name << ", You have  " << player.HP << " HP left!" << endl;
}


void generateEnemy(createenemy enemy[])
{
    srand(time(NULL));
    for (int i = 0; i < 5; i++){
        enemy[i].enemyHP = 150;
        enemy[i].weapon.name = "Killer Axe";
        enemy[i].weapon.might = 14;
        enemy[i].weapon.accuracy  = 75;
        enemy[i].weapon.critical = 25;
        enemy[i].stats.attack = rand() % 5 + 20;
        enemy[i].stats.skill = rand() % 5 + 20;
        enemy[i].stats.speed = rand() % 5 + 20;
        enemy[i].stats.luck = rand() % 5 + 20;
        enemy[i].stats.defense = rand() % 5 + 20;
        enemy[i].stats.avoid = (enemy[i].stats.speed * 2) + enemy[i].stats.luck;
        enemy[i].stats.totalAttack =  enemy[i].stats.attack + enemy[i].weapon.might;
        enemy[i].stats.hitrate = (enemy[i].stats.skill * 2) + enemy[i].weapon.accuracy + enemy[i].stats.luck;
        enemy[i].stats.critrate = enemy[i].weapon.critical + enemy[i].stats.skill;
    }

    /*(for(int i = 0; i < 5; i++){
        cout << enemy[i].name << " stats are: " << endl;
        cout << "Attack: " << enemy[i].stats.attack << endl;
        cout << "Skill: " << enemy[i].stats.skill << endl;
        cout << "Speed: " << enemy[i].stats.speed << endl;
        cout << "Luck: " << enemy[i].stats.luck << endl;
        cout << "Defense: " << enemy[i].stats.defense << endl;
        cout << "total attack: " << enemy[i].stats.totalAttack << endl;
        cout << "effective hit rate: " << enemy[i].stats.effectiveHitrate << endl;
        cout << "effective crit rate: " << enemy[i].stats.effectiveHitrate << endl;
        cout << "avoid: " << enemy[i].stats.avoid << endl;
    }*/
}

void generateBoss(createboss boss)
{
    boss.name = "EISENBERG";

    boss.weapon.name = "Gae Bolg";
    boss.weapon.accuracy = 100;
    boss.weapon.might = 20;
    boss.weapon.critical = 15;

    srand(time(0));
    boss.stats.attack = rand() % 5 + 30; //generates a random num between 30 and 35, which the bosses stats!
    boss.stats.skill = rand() % 5 + 30;
    boss.stats.speed = rand() % 5 + 30;
    boss.stats.luck = rand() % 5 + 30;
    boss.stats.defense = rand() % 5 + 30;
    boss.stats.totalAttack = boss.stats.attack + boss.weapon.might;
    boss.stats.hitrate = (boss.stats.skill * 2) + boss.weapon.accuracy + boss.stats.luck;
    boss.stats.critrate =  boss.weapon.critical + boss.stats.skill;


    //I decided to give the boss only one skill, so he would not be too overpowered
    boss.boss_skill.skillname = "Luna";
    boss.boss_skill.description = "Negates enemy defense!";
    boss.boss_skill.likelyhood = boss.stats.skill;
}

void upgradeWeapon(createplayer &player)
{
    string weaponname[3]{"Wo Dao", "Gold Axe", "silver Lance"};
    int weaponmight[3]{15, 25, 20};
    int weaponaccuracy[3]{95, 80, 85};
    int weaponcritical[3]{35, 10, 25}, choice;


    cout << "Congratulations! You've earned the right to upgrade your weapon!" << endl;
    for(int i = 0; i < 3; i++){
        cout << i+1 << ") Name: " << weaponname[i] << endl;
        cout << "Might: " << weaponmight[i] << endl;
        cout << "Accuracy: " << weaponaccuracy[i] << "%" << endl;
        cout << "Critical: " << weaponcritical[i] << "%" << endl << endl;
    }

    cout << "Which weapon do you to upgrade to?: ";
    cin >> choice;

    if (choice == 1){
        player.weapon.name = "Wo Dao";
        player.weapon.might = 15;
        player.weapon.accuracy = 95;
        player.weapon.critical = 40;
    }else if (choice == 2){
        player.weapon.name = "Gold Axe";
        player.weapon.might = 25;
        player.weapon.accuracy = 80;
        player.weapon.critical = 10;
    }else{
        player.weapon.name = "Silver Lance";
        player.weapon.might = 20;
        player.weapon.accuracy = 85;
        player.weapon.critical = 25;
    }

    cout << "You now have a " << player.weapon.name << "!" << endl;
}
