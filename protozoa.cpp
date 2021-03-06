#include "Code_Utilities.h" //travail de Khalil
#include "asciiIntro.h" //travail de Nicholas et Alexis
#include "asciiLogo.h" //travail de Nicholas
#include "asciiProtazoid.h" //travail de Nicholas
#include "asciiCollectables.h" //travail de Nicholas
#include "asciiMonsters.h" //travail de Nicholas
#include <chrono>
#include <thread>

using namespace BdB;
using namespace this_thread;

void moveCursor(int v, int h)   //travail de Nicholas
{
    cout << ESC + (to_string(v) + ";" + to_string(h) + "H");
}

string spaceString(int length)  //travail de Nicholas
{
    string spaces = "";
    for (int i = 0; i < length; ++i)
    {
        spaces += " ";
    }
    return spaces;
}

/**e
Ajoute du spacing
@param int lines Nombre de fois que move cursor sera appele
@param int sizeY position Y
@param int sizeX position X
@param xOffset(optional) si n'est pas spécifier il reste a 1
*/
void addSpacing(int lines, int sizeY, int sizeX, int vSize, int hSize, int xOffset = 1) //travail de Nicholas
{
    string spaces = spaceString(sizeX);

    for (int i = 1; i <= lines; i++) {
        moveCursor(vSize / 2 + sizeY / 2 + i, hSize / 2 - sizeX / 2 + xOffset);
        cout << spaces;
    }
}

void clearCin() //travail de Nicholas
{
    cin.clear();
    cin.ignore(120, '\n');
}

void calibrateScreen(int& vSize, int& hSize) //travail de Alexis
{
    cout << "Assurez-vous que votre écran de terminal soit maximisé.\n"
        << "Pour une meilleure expérience graphique, mettez votre échelle d'affichage à 100% (windows scale and layout)\n";
    sleep_for(2000ms);
}

//tentative de générateur de carte aléatoire qui ne fait pas tout à fait l'effet souhaité, mais utilisable
array<string, 200> buildRandomMap(int lineSize) //codé par Alexis, testé par Nicholas
{
    int threshold = 100; //modifiable
    array<string,200> newMap;
    string newLine = "";
    string lastLine = "";
    array<int, 800> cellThreshold = {};
    array<int, 800> cellThresholdLastLine = {};
    int distancer = 10; //modifiable

    for (int i = 0; i < 200; ++i)
    {
        newLine = "";
        for (int i = 0; i < ((lineSize / 2) - 1); ++i)
        {
            if (i != 0)
                threshold -= cellThreshold[i - 1] * 4; //modifiable
            threshold -= cellThresholdLastLine[i] * 30; //modifiable
            threshold = threshold <= 0 ? 5 : threshold; //modifiable
            if (rand() % threshold == threshold - 1 && distancer > 0) //formula modifiable
            {
                cellThreshold[i] += 1; //modifiable
                newLine += "1";
                distancer -= 1; //modifiable
                threshold = 100; //modifiable
            }
            else
            {
                cellThreshold[i] += 0; //modifiable
                newLine += ".";
                threshold -= 1; //modifiable
                distancer -= 1; //modifiable
            }
            distancer = distancer < -30 ? 10 : distancer; //modifiable
        }
        newMap[i] = newLine;
        lastLine = newLine;
        cellThresholdLastLine = cellThreshold;
    }
    return newMap;
}

//fonction pour demander à l'usager si il veut commencer une nouvelle partie
bool displayStartGame(int vSize, int hSize, int longestString)  //codé par Alexis, débogué par Nicholas
{
    char userStart = -1;
    int vPos = vSize / 2 + int(logo.size()) / 2;
    int hPos = hSize / 2 - longestString / 2 + 1;
    while (userStart != 'o' && userStart != 'O' && userStart != 'n' && userStart != 'N')
    {
        addSpacing(3, int(logo.size()), longestString, vSize, hSize);
        if (userStart != -1)
        {
            moveCursor(vPos + 1, hPos);
            cout << "Je n'ai pas compris, pouvez-vous répéter? ";
        }
        moveCursor(vPos + 2, hPos + 1);
        cout << "Voulez-vous débuter un nouveau Protazoid? o pour débuter.";
        moveCursor(vPos + 3, hPos + 1);
        cin >> userStart;
        clearCin();
    }
    addSpacing(1, int(logo.size()), longestString, vSize, hSize);
    bool startGame = userStart == 'o' || userStart == 'O' ? true : false;
    return startGame;
}

//fonction pour demander à l'usager d'entrer le nom de son Protazoid
string enterName(int vSize, int hSize, int longestString)   //codé pas Alexis, débogué par Nicholas
{
    string startName = "En tant que Protazoid, quel sera votre nom?";
    string startGod = "Vous pouvez mettre godmode pour commencer au plus fort.";

    int vPos = vSize / 2 + int(logo.size()) / 2;
    int hPos = hSize / 2 - int(startName.size()) / 2 + 2;

    addSpacing(3, int(logo.size()), longestString, vSize, hSize);

    moveCursor(vPos + 1, hSize / 2 - int(startGod.size()) / 2 + 2);
    cout << spaceString(int(startGod.size()));
    moveCursor(vPos + 1, hSize / 2 - int(startGod.size()) / 2 + 2);
    cout << startGod;
    moveCursor(vPos + 2, hPos);
    cout << startName;
    string testName = "";
    moveCursor(vPos + 3, hPos);
    cin >> testName;
    clearCin();
    return testName;
}

//fonction pour que l'usager valide le nom de son Protazoid
void startNameValidation(int vSize, int hSize, string& protName, int& longestString)    //codé par Alexis, testé par Nicholas
{
    bool nameIsGood = false;
    while (!nameIsGood)
    {
        string testName = enterName(vSize, hSize, longestString);
        string startNameFinalValidate = "Votre Protazoid se nomme bien " + testName + "? Tappez o pour oui.";
        longestString = startNameFinalValidate.size() > longestString ? startNameFinalValidate.size() : longestString;
        int vPos = vSize / 2 + int(logo.size()) / 2;
        int hPos = hSize / 2 - longestString / 2;
        char validateName;
                
        addSpacing(3, int(logo.size()), longestString + 2, vSize, hSize, 2);
        moveCursor(vPos + 2, hPos + 2);
        cout << startNameFinalValidate;
        moveCursor(vPos + 3, hPos + 2);
        cin >> validateName;
        clearCin();
        nameIsGood = validateName == 'O' || validateName == 'o' ? true : false;
        if(nameIsGood)
            protName = testName;
    }
}


//fonction pour afficher le texte dans l'intro
void displayIntroText(int vSize, int hSize, int textIndex)  //codé par Alexis
{
    array<string, 6> introTexts = {
        "Au tout début des temps, avant la naissance des continents et des mammifères terrestres, il n'y avait qu'océan.",
        "Immense, profond et périlleux il était. Un combat de tous les instants s’y passait. La loi du plus fort y régnait.",
        "Dans ce jeu vous incarnez un Protazoid, un être unicellulaire microscopique tout comme nos ancêtres lointains.",
        "Et tout comme eux, votre existence sera un combat de tous les instants.",
        "Comme Darwin décrit si bien \"It is not the strongest of the species that survives, nor the most intelligent; it is the one most adaptable to change.",
        "Donc, accumulez des calories en dévorant ce qui vous entoure et choisissez bien votre chemin évolutif, car le futur de votre espèce est entre vos mains." };
    int longest = int(introTexts[0].size());
    for (int i = 1; i<int(introTexts.size()); ++i)
    {
        longest = int(introTexts[i].size()) > longest ? int(introTexts[i].size()) : longest;
    }
    int vPos = vSize / 2 - logo.size() / 2;
    int hPos = hSize / 2 - longest / 2;

    moveCursor(vPos - (12 - textIndex), hPos);
    cout << spaceString(longest);
    moveCursor(vPos - (12 - textIndex), hSize / 2 - int(introTexts[textIndex].size()) / 2 + 1);
    cout << introTexts[textIndex];
}

//fonction pour afficher le Logo dans l'écran d'affichage
void displayLogo(int vSize, int hSize, int logoIndex, int logoVOffset = 0)  //codé par Alexis et Nicholas
{
    array<array<string, 16>, 6> logos = { logo6, logo5, logo4, logo3, logo2, logo };
    int longest = int(logo[0].size());
    int vPos = vSize / 2 - int(logo.size()) / 2 + logoVOffset;
    int hPos = hSize / 2 - longest / 2;

    for (int i = 0; i < logos[logoIndex].size(); ++i)
    {
        moveCursor(vPos + i, hPos);
        cout << logos[logoIndex][i];
    }
}

//fonction pour afficher l'intro
void displayIntro(int framesPlayed, bool skipIntro, int vSize, int hSize)   //codé par Alexis
{
    for (int i = 0; i < framesPlayed; ++i)
    {
        if (i == 0 || (i > 0 && !skipIntro) || i == framesPlayed - 1)
        {
            cout << "\x1b[48;5;234m" << "\x1b[38;5;76m";//couleur fonctionne juste pour l'intro
            moveCursor(1, 1);
            cout << frames[i];

            //affichage du texte d'introduction
            if (i >= 1) displayIntroText(vSize, hSize, 0);
            if (i >= 8) displayIntroText(vSize, hSize, 1);
            if (i >= 15) displayIntroText(vSize, hSize, 2);
            if (i >= 22) displayIntroText(vSize, hSize, 3);
            if (i >= 29) displayIntroText(vSize, hSize, 4);
            if (i >= 36) displayIntroText(vSize, hSize, 5);

            //affichage du logo
            if (i == framesPlayed - 6) displayLogo(vSize, hSize, 0);
            if (i == framesPlayed - 5) displayLogo(vSize, hSize, 1);
            if (i == framesPlayed - 4) displayLogo(vSize, hSize, 2);
            if (i == framesPlayed - 3) displayLogo(vSize, hSize, 3);
            if (i == framesPlayed - 2) displayLogo(vSize, hSize, 4);
            if (i == framesPlayed - 1) displayLogo(vSize, hSize, 5);

            sleep_for(200ms);
        }
    }
}

//fonction pour faire les choix des caractéristique à la création du personnage
void validateCaracteristics(int& protLife, int& protLifeMax, int& protSpeed, int& protStrength, string protName, int vSize, int hSize) //codé par Alexis
{
    bool pointsAreGood = false;
    int pointsLeft;
    while (!pointsAreGood)
    {
        pointsLeft = 4;
        protLife = 2;
        protLifeMax = 2;
        protSpeed = 2;
        protStrength = 2;

        string longestString = "Entrez le nombre de points que vous voulez ajouter à la Vitesse de votre Protazoid: (points restants  )";
        string spaces = spaceString(longestString.size());

        //VIE
        addSpacing(9, logo.size(), longestString.size(), vSize, hSize);

        moveCursor(vSize / 2 + logo.size() / 2 + 2, hSize / 2 - longestString.size() / 2 + 2);
        cout << "Points de caractéristique de " << protName;
        moveCursor(vSize / 2 + logo.size() / 2 + 3, hSize / 2 - longestString.size() / 2 + 2);
        cout << ">Vie: " << setw(3) << protLife;
        moveCursor(vSize / 2 + logo.size() / 2 + 4, hSize / 2 - longestString.size() / 2 + 3);
        cout << "Vitesse: " << setw(3) << protSpeed;
        moveCursor(vSize / 2 + logo.size() / 2 + 5, hSize / 2 - longestString.size() / 2 + 3);
        cout << "Force: " << setw(3) << protStrength;

        char usePoints = '|';
        while (int(usePoints) < int('0') || int(usePoints) > int('0' + pointsLeft))
        {
            moveCursor(vSize / 2 + logo.size() / 2 + 7, hSize / 2 - longestString.size() / 2 + 1);
            cout << spaces;
            moveCursor(vSize / 2 + logo.size() / 2 + 7, hSize / 2 - longestString.size() / 2 + 2);
            cout << "Entrez le nombre de points que vous voulez ajouter à la Vie de votre Protazoid: (points restants " << pointsLeft << ")";
            moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 2);
            if (int(usePoints) != char('|'))
            {
                moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 2);
                cout << spaces;
                moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 1);
                cout << "Entrée invalide, utilisez vos points restants.";
            }
            cin >> usePoints;
            clearCin();
        }
        int pointsUsed;
        switch (int(usePoints))
        {
            case int('0') :
                pointsUsed = 0;
                break;
            case int('1') :
                pointsUsed = 1;
                break;
            case int('2') :
                pointsUsed = 2;
                break;
            case int('3') :
                pointsUsed = 3;
                break;
            case int('4') :
                pointsUsed = 4;
                break;
        }
        pointsLeft -= pointsUsed;
        protLife += pointsUsed;

        //VITESSE
        addSpacing(9, logo.size(), longestString.size(), vSize, hSize);

        moveCursor(vSize / 2 + logo.size() / 2 + 2, hSize / 2 - longestString.size() / 2 + 2);
        cout << "Points de caractéristique de " << protName;
        moveCursor(vSize / 2 + logo.size() / 2 + 3, hSize / 2 - longestString.size() / 2 + 3);
        cout << "Vie: " << setw(3) << protLife;
        moveCursor(vSize / 2 + logo.size() / 2 + 4, hSize / 2 - longestString.size() / 2 + 2);
        cout << ">Vitesse: " << setw(3) << protSpeed;
        moveCursor(vSize / 2 + logo.size() / 2 + 5, hSize / 2 - longestString.size() / 2 + 3);
        cout << "Force: " << setw(3) << protStrength;

        usePoints = '|';
        while (int(usePoints) < int('0') || int(usePoints) > int('0' + pointsLeft))
        {
            moveCursor(vSize / 2 + logo.size() / 2 + 7, hSize / 2 - longestString.size() / 2 + 1);
            cout << spaces;
            moveCursor(vSize / 2 + logo.size() / 2 + 7, hSize / 2 - longestString.size() / 2 + 2);
            cout << "Entrez le nombre de points que vous voulez ajouter à la Vitesse de votre Protazoid: (points restants " << pointsLeft << ")";
            moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 2);
            if (int(usePoints) != char('|'))
            {
                moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 2);
                cout << spaces;
                moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 1);
                cout << "Entrée invalide, utilisez vos points restants.";
            }
            cin >> usePoints;
            clearCin();
        }
        pointsUsed = NULL;
        switch (int(usePoints))
        {
            case int('0') :
                pointsUsed = 0;
                break;
            case int('1') :
                pointsUsed = 1;
                break;
            case int('2') :
                pointsUsed = 2;
                break;
            case int('3') :
                pointsUsed = 3;
                break;
            case int('4') :
                pointsUsed = 4;
                break;
        }
        pointsLeft -= pointsUsed;
        protSpeed += pointsUsed;

        //FORCE
        addSpacing(9, logo.size(), longestString.size(), vSize, hSize);

        moveCursor(vSize / 2 + logo.size() / 2 + 2, hSize / 2 - longestString.size() / 2 + 2);
        cout << "Points de caractéristique de " << protName;
        moveCursor(vSize / 2 + logo.size() / 2 + 3, hSize / 2 - longestString.size() / 2 + 3);
        cout << "Vie: " << setw(3) << protLife;
        moveCursor(vSize / 2 + logo.size() / 2 + 4, hSize / 2 - longestString.size() / 2 + 3);
        cout << "Vitesse: " << setw(3) << protSpeed;
        moveCursor(vSize / 2 + logo.size() / 2 + 5, hSize / 2 - longestString.size() / 2 + 2);
        cout << ">Force: " << setw(3) << protStrength;

        usePoints = '|';
        while (int(usePoints) < int('0') || int(usePoints) > int('0' + pointsLeft))
        {
            moveCursor(vSize / 2 + logo.size() / 2 + 7, hSize / 2 - longestString.size() / 2 + 1);
            cout << spaces;
            moveCursor(vSize / 2 + logo.size() / 2 + 7, hSize / 2 - longestString.size() / 2 + 2);
            cout << "Entrez le nombre de points que vous voulez ajouter à la Force de votre Protazoid: (points restants " << pointsLeft << ")";
            moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 2);
            if (int(usePoints) != char('|'))
            {
                moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 2);
                cout << spaces;
                moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 1);
                cout << "Entrée invalide, utilisez vos points restants.";
            }
            cin >> usePoints;
            clearCin();
        }
        pointsUsed = NULL;
        switch (int(usePoints))
        {
            case int('0') :
                pointsUsed = 0;
                break;
            case int('1') :
                pointsUsed = 1;
                break;
            case int('2') :
                pointsUsed = 2;
                break;
            case int('3') :
                pointsUsed = 3;
                break;
            case int('4') :
                pointsUsed = 4;
                break;
        }
        pointsLeft -= pointsUsed;
        protStrength += pointsUsed;

        addSpacing(9, int(logo.size()), int(longestString.size()), vSize, hSize);

        moveCursor(vSize / 2 + int(logo.size()) / 2 + 2, hSize / 2 - int(longestString.size()) / 2 + 2);
        cout << "Points de caractéristique de " << protName;
        moveCursor(vSize / 2 + int(logo.size()) / 2 + 3, hSize / 2 - int(longestString.size()) / 2 + 3);
        cout << "Vie: " << setw(3) << protLife;
        moveCursor(vSize / 2 + int(logo.size()) / 2 + 4, hSize / 2 - int(longestString.size()) / 2 + 3);
        cout << "Vitesse: " << setw(3) << protSpeed;
        moveCursor(vSize / 2 + int(logo.size()) / 2 + 5, hSize / 2 - int(longestString.size()) / 2 + 3);
        cout << "Force: " << setw(3) << protStrength;

        char validCarac;
        moveCursor(vSize / 2 + logo.size() / 2 + 7, hSize / 2 - longestString.size() / 2 + 2);
        cout << "Est-ce que les caractéristique de votre Protazoid vous conviennent? o pour oui.";
        moveCursor(vSize / 2 + logo.size() / 2 + 8, hSize / 2 - longestString.size() / 2 + 2);
        cin >> validCarac;
        clearCin();
        pointsAreGood = validCarac == 'o' || validCarac == 'O' ? true : false;
    }
    protLifeMax = protLife;
}

//fonction pour afficher le texte du protazoid sur plus d'une trame
void displayUI(array<string, 10>& UItexts, array<int, 10>& UIsteps, array<int, 20>& UIpositions, int protLife, int bossHealth)    //codé par Alexis
{
    if (protLife > 0 && bossHealth > 0)
    {
        for (int i = 0; i < 10; ++i)
        {
            string textDisplayed = UItexts[i];
            if (textDisplayed != "")
            {
                int stepsleft = UIsteps[i];
                int vUI = UIpositions[i * 2];
                int hUI = UIpositions[(i * 2) + 1];
                moveCursor(vUI, hUI);
                cout << textDisplayed;
                --stepsleft;
                if (stepsleft <= 0)
                {
                    UItexts[i] = "";
                }
                else
                {
                    UIsteps[i] = stepsleft;
                }
            }
        }
    }
}

//fonction pour ajouter un text a afficher dans le UI       //codé par Alexis
void addTextToUI(int vPos, int hPos, string uiString, array<string, 10>& UItexts, array<int, 10>& UIsteps, array<int, 20>& UIpositions, int vUIOffset = 1, int hUIOffset = 0)
{
    moveCursor(vPos - vUIOffset, hPos - hUIOffset);
    cout << uiString;
    for (int i = 0; i < 10; ++i)
    {
        if (UItexts[i] == "")
        {
            UItexts[i] = uiString;
            UIsteps[i] = 3;
            UIpositions[i * 2] = vPos - vUIOffset;
            UIpositions[(i * 2) + 1] = hPos - hUIOffset;
            continue;
        }
    }
}

//appelé lorsque la vie du Protazoid atteint 0, joue l'animation de mort du Protazoid
void playProtazoidDeath(int vSize, int hSize, int protLife) //codé par Alexis et Nicholas
{
    int spriteSize = 0;
    switch (protLife)
    {
    case -1:
        spriteSize = int(ProtagAdyingA.size());
        break;
    case -2:
        spriteSize = int(ProtagAdyingB.size());
        break;
    case -3:
        spriteSize = int(ProtagAdyingC.size());
        break;
    case -4:
        spriteSize = int(ProtagAdyingD.size());
        break;
    case -5:
        spriteSize = int(ProtagAdyingE.size());
        break;
    default:
        break;
    }
    int vPos = vSize / 2 - spriteSize / 2;
    int hPos = hSize / 2 - spriteSize / 2;
    for (int i = 0; i < spriteSize; ++i)
    {
        moveCursor(vPos + i, hPos);
        switch (protLife)
        {
        case -1:
            cout << ProtagAdyingA[i];
            break;
        case -2:
            cout << ProtagAdyingB[i];
            break;
        case -3:
            cout << ProtagAdyingC[i];
            break;
        case -4:
            cout << ProtagAdyingD[i];
            break;
        case -5:
            cout << ProtagAdyingE[i];
            break;
        default:
            break;
        }
    }
}

//mode mort subite quand le Protazoid sort de la carte
void suddenDeath(int vSize, int hSize, int& suddenDeathCtr, array<string,10>& UItexts, array<int, 10>& UIsteps, array<int, 20>& UIpositions, int protHeight, int protWidth) //codé par Alexis
{
    int hPos = hSize / 2 - protWidth / 2;
    int vPos = vSize / 2 - protHeight / 2;
    //affiche message d'avertissement et décrémente le compteur
    string uiString = "Attention!! Retourner sur la carte, le médium n'est pas compatible avec votre métabolisme!";
    addTextToUI(vPos, hPos, uiString, UItexts, UIsteps, UIpositions, 6, int(uiString.size()) / 2);
    --suddenDeathCtr;
    moveCursor(vPos + (protHeight / 2) + 2, hPos);
    cout << spaceString(2);
    moveCursor(vPos + protHeight + 2, hPos + (protWidth / 2));
    cout << suddenDeathCtr;

}

//dessine le model de protazoid nécéssaire et vérifie plusieurs systèmes de vie du protazoid
bool drawProtazoid(int vSize, int hSize, int protWidth, int protHeight, int& protLife, int& protLifeMax, bool protRegen, int& protSpeed, int& protStrength, int& protSatiety, array<int,4>& protInvQty, array<string, 10>& UItexts, array<int, 10>& UIsteps, array<int, 20>& UIpositions) //codé par Alexis et par Nicholas vers la fin du projet
{
    int hPos = hSize / 2 - protWidth / 2;
    int vPos = vSize / 2 - protHeight / 2;
    if (protLife > 0)
    {
        //affiche un Protazoid au centre de l'écran d'affichage
        if (protLifeMax >= 10) //utilise le plus gros model si la vie est plus grande ou egal à 10
        {
            for (int i = 0; i < ProtagAlarge.size(); ++i)
            {
                moveCursor(vPos + i, hPos);
                cout << ProtagAlarge[i];
            }
        }
        else if (protLifeMax > 6) //utilise le model medium si la vie est plus grande ou egal à 6
        {
            for (int i = 0; i < ProtagAmedium.size(); ++i)
            {
                moveCursor(vPos + i, hPos);
                cout << ProtagAmedium[i];
            }
        }
        else //utilise le petit model si la vie est plus petite que 6
        {
            for (int i = 0; i < ProtagAsmall.size(); ++i)
            {
                moveCursor(vPos + i, hPos);
                cout << ProtagAsmall[i];
            }
        }
    }
    else  //si le Protazoid a 0 pv, jouer l'anim de mort et afficher l'écran de fin de jeu
    {
        --protLife;

        //joue l'animation de mort du Protazoid
        if (protLife <= -1 && protLife >= -5) playProtazoidDeath(vSize, hSize, protLife);

        //joue l'animation du logo vers la fin de l'animation du Protazoid
        int logoIndex = -protLife - 4;
        if (protLife <= -4 && protLife >= -9) displayLogo(vSize, hSize, logoIndex, -int(logo.size()));
    }

    //la satiété baisse à chaque pas, en dessous de 50, utilise des calories pour remplir la satiété jusqu'à 100 points(10 points par calories)
    protSatiety -= 1;
    if (protSatiety < 50 && protInvQty[0] > 0)
    {
        int usedCal = protInvQty[0] < (100 - protSatiety) / 10 ? protInvQty[0] : (100 - protSatiety) / 10;
        protInvQty[0] -= usedCal;
        protSatiety += usedCal * 10;
        string uiString = string("\x1b[48;5;11m\x1b[38;5;16m") + to_string(usedCal) + " dépensés";
        addTextToUI(vPos, hPos, uiString, UItexts, UIsteps, UIpositions, 1, 2);
    }
    else if (protSatiety < 50 && protSatiety > 0 &&  protInvQty[0] == 0)
    {
        if (protSatiety > 40 || protSatiety < 5)
        {
            moveCursor(vPos - 1, hPos - 20);
            cout << "\x1b[48;5;11m\x1b[38;5;16m" << " Vous n'avez plus d'énergie, trouvez quelque chose à manger vite!!";
        }
    }
    else if (protSatiety <= -10)
    {
        protLife -= 1;
        moveCursor(vPos - 1, hPos - 20);
        cout << "\x1b[48;5;11m\x1b[38;5;16m" << "Chaque pas est de plus en plus lourd, il faut de l'énergie!! -1 Vie (" << protLife << ")";
        protSatiety = 0;
    }

    //si le protazoid est en évolution et a des calories, il doit payer tout ce qu'il peut jusqu'à ce que l'évolution soit finie
    int evoluting = -1;
    int evolutionETA = 0;
    for (int i = 1; i <= 3; ++i)
    {
        if (protInvQty[i] > 0)
        {
            evoluting = i;
            evolutionETA = protInvQty[i];
        }
    }
    if (evolutionETA != 0)
    {
        int calUsed = protInvQty[0];
        evolutionETA -= protInvQty[0];
        if (evolutionETA <= 0)
        {
            calUsed += evolutionETA;
            if (evoluting == 1)
            {
                protLife += 1;
                protLifeMax += 1;
            }
            if (evoluting == 2)
            {
                protStrength += 1;
            }
            if (evoluting == 3)
            {
                protSpeed += 1;
            }
        }
        protInvQty[0] -= calUsed;
        protInvQty[evoluting] -= calUsed;
    }

    //si le protazoid n'attaque pas, il peut regénérer
    if (protLife < protLifeMax && protInvQty[0] > 0 && protRegen && protLife > 0)
    {
        protLife += 1;
        protInvQty[0] -= 1;
        string uiString = string("\x1b[48;5;11m\x1b[38;5;16m") + "calories(" + to_string(protInvQty[0]) + ") for life(" + to_string(protLife) + ")";
        addTextToUI(vPos, hPos, uiString, UItexts, UIsteps, UIpositions, 3);
    }

    return true;
}

//fonction pour mettre les positions des monstres dans la carte
void positionMonsters(int lineSize, array<string, 200> currentMap, array<int, 20>& monstersPosition, array<int, 10>& monstersHealth, array<int, 10>& monstersStrength) //codé par Alexis
{
    int monstersNumber = 10;
    for (int i = 0; i < monstersNumber; ++i)
    {
        int h = 1 + (rand() % lineSize);
        int v = 1 + (rand() % int(currentMap.size()));
        monstersPosition[i * 2] = h;
        monstersPosition[(i * 2) + 1] = v;
        monstersHealth[i] = 4;
        monstersStrength[i] = 3;
    }
}

//fonction pour faire bouger les monstres et changer leur position sur la carte
void moveMonsters(int vOffset, int hOffset, int lineSize, array<string, 200> currentMap, array<int, 20>& monstersPosition, array<int, 10> monstersHealth) //codé par Alexis
{
    int monstersNumber = 10;
    for (int i = 0; i < monstersNumber; ++i)
    {
        if (monstersHealth[i] <= 0) continue;
        int spdBoost = 1;
        if (rand() % 3 == 2) spdBoost = 5;
        if (rand() % 5 == 4)
        {
            int h = monstersPosition[i * 2];
            int v = monstersPosition[(i * 2) + 1];
            int monsterVOffset = v - int(currentMap.size())/2;
            int monsterHOffset = h - lineSize / 2;
            int moveV = monsterVOffset < vOffset ? 1 : -1;
            int moveH = monsterHOffset < hOffset ? 1 : -1;
            monstersPosition[i * 2] = h + ((moveH * 1.5) * spdBoost);
            monstersPosition[(i * 2) + 1] = v + (moveV * spdBoost);
        }
        else
        {
            monstersPosition[i * 2] = monstersPosition[i * 2] + ((rand() % 3) - 1) * spdBoost;
            monstersPosition[(i * 2) + 1] = monstersPosition[(i * 2) + 1] + ((rand() % 3) - 1) * spdBoost;
        }
    }
}

//fonction pour afficher l'animation de mort des monstres
void playMonsterDeath(int vSize, int hSize, int monsterHP, int v, int h, int monsterVOffset, int monsterHOffset, int vOffset, int hOffset, int& monsterHStart, int monsterVStart, int monsterVEnd, int monsterHEnd) //codé par Alexis
{
    int spriteSize = 0;
    string line;
    int lineLength;
    int vPos = vSize / 2 - spriteSize / 2;
    int hPos = hSize / 2 - spriteSize / 2;
    monsterHStart = monsterHStart > (spriteSize * 2) || monsterHStart < 0 ? 0 : monsterHStart;
    for (int iLine = monsterVStart; iLine < monsterVEnd; ++iLine)
    {
        switch (monsterHP)
        {
        case -1:
            spriteSize = int(monsterAsmall.size());
            line = monsterAsmall[iLine];
            break;
        case -2:
            spriteSize = int(monsterAdyingA.size());
            line = monsterAdyingA[iLine];
            break;
        case -3:
            spriteSize = int(monsterAdyingB.size());
            line = monsterAdyingB[iLine];
            break;
        case -4:
            spriteSize = int(monsterAdyingC.size());
            line = monsterAdyingC[iLine];
            break;
        default:
            break;
        }
        string newLine;
        int notAnsiCounter = 0;
        for (int iCol = 0; iCol < line.size(); ++iCol)
        {
            if (line[iCol] == ' ' || line[iCol] == '.')
            {
                if (notAnsiCounter < monsterHEnd)
                {
                    if (notAnsiCounter >= monsterHStart)
                        newLine += line[iCol];
                }
                ++notAnsiCounter;
            }
            else if (notAnsiCounter < monsterHEnd)
                newLine += line[iCol];
        }
        int hMove = h - hOffset < monsterHOffset ? 1 : h - monsterHOffset - hOffset;
        moveCursor(v + iLine - monsterVOffset - vOffset, hMove);
        cout << newLine;
    }
}

//fonction pour afficher les monstres à l'écran d'affichage
void drawMonsters(int vSize, int hSize, int vOffset, int hOffset, int lineSize, array<string, 200> currentMap, int protWidth, int protHeight, int& protLife, bool& protRegen, int protStrength, array<int, 4>& protInvQty, array<int, 20>& monstersPosition, array<int, 10>& monstersHealth, array<int, 10> monstersStrength) //codé par Alexis
{
    for (int iMonster = 0; iMonster < 10; ++iMonster)
    {
        int h = monstersPosition[iMonster * 2];
        int v = monstersPosition[(iMonster * 2) + 1];
        int monsterVOffset = (int(currentMap.size()) / 2) - (vSize / 2);
        int monsterVMaxOffset = (int(currentMap.size()) / 2) + (vSize / 2);
        int monsterVStart = int(monsterAbig.size()) - ((v + int(monsterAbig.size())) - ((int(currentMap.size()) / 2) - (vSize / 2) + vOffset));
        int monsterVEnd = int(monsterAbig.size()) - (v - ((int(currentMap.size()) / 2) + (vSize / 2) + vOffset) + int(monsterAbig.size()) - 1);
        int monsterHOffset = (lineSize / 2) - (hSize / 2);
        int monsterHMaxOffset = (lineSize / 2) + (hSize / 2);
        int monsterHStart = 16 - ((h + 16) - ((lineSize / 2) - (hSize / 2) + hOffset));
        int hStartLimit = ((lineSize / 2) - (hSize / 2) + hOffset);
        int monsterHEnd = hSize - (h - hStartLimit) > 16 ? 16 : hSize - (h - hStartLimit) + 1;
        int monsterStr = monstersStrength[iMonster];
        int monsterHP = monstersHealth[iMonster];
        int protTop = (vSize / 2) - (protHeight / 2);
        int protBottom = (vSize / 2) + (protHeight / 2);
        int protLeft = (hSize / 2) - (protWidth / 2);
        int protRight = (hSize / 2) + (protWidth / 2);

        //determiner si le monstre est dans l'écran d'affichage
        if(monsterVStart < int(monsterAbig.size()) && monsterVEnd > 0)
        {
            monsterVStart = monsterVStart > int(monsterAbig.size()) || monsterVStart < 0 ? 0 : monsterVStart;
            monsterVEnd = int(monsterAbig.size()) > monsterVEnd ? monsterVEnd : int(monsterAbig.size());
            if (monsterHStart < 16 && h < ((lineSize / 2) + (hSize / 2) + hOffset))
            {
                //determiner si le monstre est en collision avec le Protazoid
                if (monsterHP > 0 && protLife > 0) {
                    if ((v - monsterVOffset - vOffset) > protTop && (v - monsterVOffset - vOffset) < protBottom)
                    {
                        if ((h - monsterHOffset - hOffset) > protLeft && (h - monsterHOffset - hOffset) < protRight)
                        {
                            moveCursor(protTop - 4, protLeft);
                            int protAttack = (protStrength + 1) + rand() % 6;
                            int monsterAttack = (monsterStr + 1) + rand() % 6;
                            //si l'attaque du prot réussi, afficher le message approprié et ajuster les pv du monstres
                            if (protAttack >= monsterAttack)
                            {
                                monsterHP -= 1;
                                monstersHealth[iMonster] = monsterHP;
                                string hit = string("\x1b[48;5;11m\x1b[38;5;16m") + "Bang!! (" + to_string(monsterHP) + ")";
                                string win = string("\x1b[48;5;11m\x1b[38;5;16m") + "Miam!! (" + to_string(monsterHP) + ")";
                                string attackresult = monsterHP <= 0 ? win : hit;
                                cout << attackresult;
                            }
                            //si l'attaque du prot est ratée, ajuster les pv du prot et afficher un message
                            else
                            {
                                protLife -= 1;
                                cout << "\x1b[48;5;160m\x1b[38;5;15m" << "Ouch!! (" << protLife << ")";
                            }
                            protRegen = false;
                        }
                    }
                }

                //si le monstre est encore en vie, afficher le monstre caractere par caractere pour éviter les débordements d'affichage
                if (monsterHP > 0)
                {
                    monsterHStart = monsterHStart > 16 || monsterHStart < 0 ? 0 : monsterHStart;
                    for (int iLine = monsterVStart; iLine < monsterVEnd; ++iLine)
                    {
                        string newLine;
                        int notAnsiCounter = 0;
                        string line = monsterAbig[iLine];
                        for (int iCol = 0; iCol < int(monsterAbig[iLine].size()); ++iCol)
                        {
                            if (line[iCol] == ' ' || line[iCol] == '.')
                            {
                                if (notAnsiCounter < monsterHEnd)
                                {
                                    if (notAnsiCounter >= monsterHStart)
                                        newLine += line[iCol];
                                }
                                ++notAnsiCounter;
                            }
                            else if(notAnsiCounter < monsterHEnd)
                                newLine += line[iCol];
                        }
                        int hMove = h - hOffset < monsterHOffset ? 1 : h - monsterHOffset - hOffset;
                        moveCursor(v + iLine - monsterVOffset - vOffset, hMove);
                        cout << newLine;
                    }
                }
                //si le monstre est mort, afficher le model de mort associé
                else
                {
                    monstersHealth[iMonster] = monsterHP - 1;
                    if (monstersHealth[iMonster] == -1)
                    {
                        protInvQty[0] += 15;
                        moveCursor(protTop - 5, protLeft + 2);
                        cout << "\x1b[48;5;11m\x1b[38;5;16m" << "Mmmmm!! +15!!! (" << protInvQty[0] << ")";
                        playMonsterDeath(vSize, hSize, monstersHealth[iMonster], v, h, monsterVOffset, monsterHOffset, vOffset, hOffset, monsterHStart, monsterVStart, monsterVEnd, monsterHEnd);
                    }
                    if (monstersHealth[iMonster] == -2)
                    {
                        moveCursor(protTop - 5, protLeft + 2);
                        cout << "\x1b[48;5;11m\x1b[38;5;16m" << "Mmmmm!! +15!! (" << protInvQty[0] << ")";
                        playMonsterDeath(vSize, hSize, monstersHealth[iMonster], v, h, monsterVOffset, monsterHOffset, vOffset, hOffset, monsterHStart, monsterVStart, monsterVEnd, monsterHEnd);
                    }
                    if (monstersHealth[iMonster] == -3)
                    {
                        moveCursor(protTop - 5, protLeft + 2);
                        cout << "\x1b[48;5;11m\x1b[38;5;16m" << "Mmmmm!! +15! (" << protInvQty[0] << ")";
                        playMonsterDeath(vSize, hSize, monstersHealth[iMonster], v, h, monsterVOffset, monsterHOffset, vOffset, hOffset, monsterHStart, monsterVStart, monsterVEnd, monsterHEnd);
                    }
                    if (monstersHealth[iMonster] == -4)
                    {
                        playMonsterDeath(vSize, hSize, monstersHealth[iMonster], v, h, monsterVOffset, monsterHOffset, vOffset, hOffset, monsterHStart, monsterVStart, monsterVEnd, monsterHEnd);
                        //quand un monstre est disparu, un nouveau est créé
                        int newMonsterH = 1 + (rand() % lineSize);
                        int newMonsterV = 1 + (rand() % int(currentMap.size()));
                        monstersPosition[iMonster * 2] = newMonsterH;
                        monstersPosition[(iMonster * 2) + 1] = newMonsterV;
                        monstersHealth[iMonster] = 4;
                        monstersStrength[iMonster] = 3;
                    }
                }
            }
        }
    }
}

//fonction pour initialiser la position des blobs sur la carte
void positionBlobs(int lineSize, array<string, 200> currentMap, int blobsNumber, array<int, 100>& blobsPosition) //codé par Alexis
{
    for (int i = 0; i < blobsNumber; ++i)
    {
        int h = rand() % lineSize;
        int v = rand() % currentMap.size();
        blobsPosition[i * 2] = h;
        blobsPosition[(i * 2) + 1] = v;
    }
}

//fonction pour initialiser les tailles des blobs
void sizeBlobs(int blobsNumber, array<int, 50>& blobsSizes) //codé par Alexis
{
    for (int i = 0; i < blobsNumber; ++i)
    {
        int randomSize = rand() % 6;
        blobsSizes[i] = randomSize;
    }
}

//fonction pour ajouter des blobs aléatoirements jusqu'a un maximum
void addBlobs(int lineSize, array<string, 200> currentMap, array<int, 100>& blobsPosition, array<int, 50>& blobsSizes)  //codé par Alexis
{
    int numberAdded = 5 + rand() % 6;
    for (int i = 0; i < 50; ++i)
    {
        int h = blobsPosition[i * 2];
        int v = blobsPosition[(i * 2) + 1];
        if (h == 0 && v == 0)
        {
            int h = rand() % lineSize;
            int v = rand() % currentMap.size();
            blobsPosition[i * 2] = h;
            blobsPosition[(i * 2) + 1] = v;
            int randomSize = rand() % 6;
            blobsSizes[i] = randomSize;
            numberAdded -= 1;
        }
        if (numberAdded <= 0) continue;
    }
}

//fonction pour dessiner un blob associé à la taille
void drawSingleBlob(int v, int h, int blobVOffset, int blobHOffset, int vOffset, int hOffset, int blobSize, int blobVStart, int blobVEnd, int blobHStart, int blobHEnd) //codé par Alexis et Nicholas
{
    for (int i = 0; i < 2; ++i)
    {
        moveCursor(v + i - blobVOffset - vOffset, h - blobHOffset - hOffset);
        switch (blobSize)
        {
        case 0:
        case 1:
        case 2:
            for (int iLine = blobVStart; iLine < blobVEnd; ++iLine)
            {
                string newLine;
                int notAnsiCounter = 0;
                string line = BlobASmall[iLine];
                for (int iCol = 0; iCol < int(BlobASmall[iLine].size()); ++iCol)
                {
                    if (line[iCol] == ' ' || line[iCol] == '.' || line[iCol] == '/' || line[iCol] == '¯' || line[iCol] == '_' || line[iCol] == '\\')
                    {
                        if (notAnsiCounter < blobHEnd)
                        {
                            if (notAnsiCounter >= blobHStart)
                                newLine += line[iCol];
                        }
                        ++notAnsiCounter;
                    }
                    else if (notAnsiCounter < blobHEnd)
                        newLine += line[iCol];
                }
                int hMove = h - hOffset < blobHOffset ? 1 : h - blobHOffset - hOffset;
                moveCursor(v + iLine - blobVOffset - vOffset, hMove);
                cout << newLine;
            }
            //cout << BlobASmall[i];
            break;
        case 3:
        case 4:
            for (int iLine = blobVStart; iLine < blobVEnd; ++iLine)
            {
                string newLine;
                int notAnsiCounter = 0;
                string line = BlobBSmall[iLine];
                for (int iCol = 0; iCol < int(BlobBSmall[iLine].size()); ++iCol)
                {
                    if (line[iCol] == ' ' || line[iCol] == '.' || line[iCol] == '/' || line[iCol] == '¯' || line[iCol] == '_' || line[iCol] == '|')
                    {
                        if (notAnsiCounter < blobHEnd)
                        {
                            if (notAnsiCounter >= blobHStart)
                                newLine += line[iCol];
                        }
                        ++notAnsiCounter;
                    }
                    else if (notAnsiCounter < blobHEnd)
                        newLine += line[iCol];
                }
                int hMove = h - hOffset < blobHOffset ? 1 : h - blobHOffset - hOffset;
                moveCursor(v + iLine - blobVOffset - vOffset, hMove);
                cout << newLine;
            }
            //cout << BlobBSmall[i];
            break;
        case 5:
            for (int iLine = blobVStart; iLine < blobVEnd; ++iLine)
            {
                string newLine;
                int notAnsiCounter = 0;
                string line = BlobAmedium[iLine];
                for (int iCol = 0; iCol < int(BlobAmedium[iLine].size()); ++iCol)
                {
                    if (line[iCol] == ' ' || line[iCol] == '.')
                    {
                        if (notAnsiCounter < blobHEnd)
                        {
                            if (notAnsiCounter >= blobHStart)
                                newLine += line[iCol];
                        }
                        ++notAnsiCounter;
                    }
                    else if (notAnsiCounter < blobHEnd)
                        newLine += line[iCol];
                }
                int hMove = h - hOffset < blobHOffset ? 1 : h - blobHOffset - hOffset;
                moveCursor(v + iLine - blobVOffset - vOffset, hMove);
                cout << newLine;
            }
            //cout << BlobAmedium[i];
            break;
        }
    }
}

//fonction pour afficher les blobs sur l'écran d'affichage
void drawBlobs(int vSize, int hSize, int vOffset, int hOffset, int lineSize, array<string, 200> currentMap, int protWidth, int protHeight, array<int, 4>& protInvQty, array<int, 100>& blobsPosition, array<int, 50> blobsSizes, int protSteps, array<string, 10>& UItexts, array<int, 10>& UIsteps, array<int, 20>& UIpositions) //codé par Alexis
{
    for (int i = 0; i < 50; ++i)
    {
        int h = blobsPosition[i * 2];
        int v = blobsPosition[(i * 2) + 1];
        if (h == 0 && v == 0) continue;

        int blobVStart = int(BlobASmall.size()) - ((v + int(BlobASmall.size())) - ((int(currentMap.size()) / 2) - (vSize / 2) + vOffset));
        int blobVEnd = int(BlobASmall.size()) - (v - ((int(currentMap.size()) / 2) + (vSize / 2) + vOffset) + int(BlobASmall.size()) - 1);
        int blobVOffset = (int(currentMap.size()) / 2) - (vSize / 2);
        int blobLineSize = blobsSizes[i] <= 2 ? 3 : (blobsSizes[i] <= 4 ? 4 : 6);
        int hStartLimit = ((lineSize / 2) - (hSize / 2) + hOffset);
        int blobHStart = blobLineSize - ((h + blobLineSize) - ((lineSize / 2) - (hSize / 2) + hOffset));
        int blobHEnd = hSize - (h - hStartLimit) > blobLineSize ? blobLineSize : hSize - (h - hStartLimit) + 1;


        int blobHOffset = (lineSize / 2) - (hSize / 2);
        moveCursor(2 + i, 1);

        //déterminer si le blob est dans l'écran d'affichage
        if (blobVStart < int(BlobASmall.size()) && blobVEnd > 0)
        {
            blobVStart = blobVStart > int(BlobASmall.size()) || blobVStart < 0 ? 0 : blobVStart;
            blobVEnd = int(BlobASmall.size()) > blobVEnd ? blobVEnd : int(BlobASmall.size());
        
        
            if (blobHStart < blobLineSize && h < (lineSize / 2) + (hSize / 2) + hOffset - blobLineSize)
            {
                //determiner si le blob a été mangé par le protazoid
                if ((v - blobVOffset - vOffset) > ((vSize / 2) - (protHeight / 2)) && (v - blobVOffset - vOffset) < ((vSize / 2) + (protHeight / 2)))
                {
                    if ((h - blobHOffset - hOffset) > ((hSize / 2) - (protWidth / 2)) && (h - blobHOffset - hOffset) < ((hSize / 2) + (protWidth / 2)))
                    {
                        blobsPosition[i * 2] = 0;
                        blobsPosition[(i * 2) + 1] = 0;
                        int vPos = vSize / 2 - protHeight / 2;
                        int hPos = hSize / 2 - protWidth / 2;
                        int blobWorth = blobsSizes[i];
                        protInvQty[0] += blobWorth + 1;
                        string uiString = string("\x1b[48;5;11m\x1b[38;5;16m") + "Slurp!! +" + to_string(blobWorth + 1) + " (" + to_string(protInvQty[0]) + ")";
                        addTextToUI(vPos, hPos, uiString, UItexts, UIsteps, UIpositions, 2);
                    }
                    else
                    {
                        blobHStart = blobHStart > blobLineSize || blobHStart < 0 ? 0 : blobHStart;
                        drawSingleBlob(v, h, blobVOffset, blobHOffset, vOffset, hOffset, blobsSizes[i], blobVStart, blobVEnd, blobHStart, blobHEnd);
                    }
                }
                else
                {
                    blobHStart = blobHStart > blobLineSize || blobHStart < 0 ? 0 : blobHStart;
                    drawSingleBlob(v, h, blobVOffset, blobHOffset, vOffset, hOffset, blobsSizes[i], blobVStart, blobVEnd, blobHStart, blobHEnd);
                }
            }
        }
    }
}

//déplace le boss vers le prot et incrémente la vitesse à tous les 100 pas
void moveBoss(int vOffset, int hOffset, int lineSize, array<string, 200> currentMap, int& bossSteps, array<int, 2>& bossPosition) //codé par Alexis
{
    int spdBoost = 1 + (bossSteps / 100);
    moveCursor(2, 2);
    int v = bossPosition[0];
    int h = bossPosition[1];
    int bossVOffset = v - int(currentMap.size()) / 2;
    int bossHOffset = h - lineSize / 2;
    int moveV = bossVOffset < vOffset ? 1 : -1;
    int moveH = bossHOffset < hOffset ? 1 : -1;
    bossPosition[0] = v + (moveV * spdBoost);
    bossPosition[1] = h + ((moveH*1.5) * spdBoost);
    ++bossSteps;
}

//affichage de l'animation de mort du boss
void playBossDeath(int vSize, int hSize, int hStartAt, int hEndAt, int vStartAt, int vEndAt, int vBoss, int hBoss, int bossVOffset, int bossHOffset, int vOffset, int hOffset, int protHeight, int protWidth, int frame)
{
    int bossALineSize = int(bossA.size());
    int bossCurrentLineSize;
    switch (frame)
    {
    case 1:
        bossCurrentLineSize = int(bossAdyingA.size());
        break;
    case 2:
        bossCurrentLineSize = int(bossAdyingB.size());
        break;
    case 3:
        bossCurrentLineSize = int(bossAdyingC.size());
        break;
    case 4:
        bossCurrentLineSize = int(bossAdyingD.size());
        break;
    case 5:
        bossCurrentLineSize = int(bossAdyingE.size());
        break;
    case 6:
        bossCurrentLineSize = int(bossAdyingF.size());
        break;
    case 7:
        bossCurrentLineSize = int(bossAdyingG.size());
        break;
    case 8:
        bossCurrentLineSize = int(bossAdyingH.size());
        break;
    }

    moveCursor(vSize / 2 - protHeight / 2 - 3, hSize / 2 - protWidth / 2 + 2);
    cout << "\x1b[48;5;11m\x1b[38;5;16m" << "Wouhou!! +1500!!!";

    //affiche la frame associé de la mort du boss caractère par caractère pour éviter les débordements d'affichage
    int hDeathStart = hStartAt - (bossALineSize * 2 - bossCurrentLineSize * 2) / 2;
    hDeathStart = hDeathStart < 0 ? 0 : hDeathStart;
    int hDeathEnd = hEndAt - (bossALineSize * 2 - bossCurrentLineSize * 2) / 2;
    int vDeathStart = vStartAt - (bossALineSize - bossCurrentLineSize) / 2;
    vDeathStart = vDeathStart < 0 ? 0 : vDeathStart;
    int vDeathEnd = vEndAt - (bossALineSize - bossCurrentLineSize) / 2;
    vDeathEnd = vDeathEnd > bossCurrentLineSize ? bossCurrentLineSize : vDeathEnd;
    int hMove = hBoss - hOffset < bossHOffset ? 1 : hBoss - bossHOffset - hOffset;
    for (int iLine = vDeathStart; iLine < vDeathEnd; ++iLine)
    {
        string newLine;
        int notAnsiCounter = 0;
        string line;
        switch (frame)
        {
        case 1:
            line = bossAdyingA[iLine];
            break;
        case 2:
            line = bossAdyingB[iLine];
            break;
        case 3:
            line = bossAdyingC[iLine];
            break;
        case 4:
            line = bossAdyingD[iLine];
            break;
        case 5:
            line = bossAdyingE[iLine];
            break;
        case 6:
            line = bossAdyingF[iLine];
            break;
        case 7:
            line = bossAdyingG[iLine];
            break;
        case 8:
            line = bossAdyingH[iLine];
            break;
        }

        for (int iCol = hDeathStart; iCol < int(line.size()); ++iCol)
        {
            if (line[iCol] == ' ' || line[iCol] == '.')
            {
                ++notAnsiCounter;
                if (notAnsiCounter < hDeathEnd)
                {
                    if (notAnsiCounter >= hDeathStart)
                        newLine += line[iCol];
                }
            }
            else newLine += line[iCol];
        }
        moveCursor(vBoss + iLine - bossVOffset - vOffset + ((bossALineSize - bossCurrentLineSize) / 2), hMove + ((bossALineSize * 2 - bossCurrentLineSize * 2) / 2));
        cout << newLine;
    }
    sleep_for(100ms);
}

//affiche le boss à l'écran
void drawBoss(int vSize, int hSize, int vOffset, int hOffset, int lineSize, array<string, 200> currentMap, int protWidth, int protHeight, int& protLife, bool& protRegen, int protStrength, array<int, 2> bossPosition, int& bossHealth, int bossStrength)  //codé par Alexis et Nicholas
{
    int vBoss = bossPosition[0];
    int hBoss = bossPosition[1];
    int bossVOffset = (int(currentMap.size()) / 2) - (vSize / 2);
    int bossHOffset = (lineSize / 2) - (hSize / 2);
    int vStartAt = int(bossA.size()) - ((vBoss + int(bossA.size())) - ((int(currentMap.size()) / 2) - (vSize / 2) + vOffset));
    int vEndAt = int(bossA.size()) - (vBoss - ((int(currentMap.size()) / 2) + (vSize / 2) + vOffset) + int(bossA.size()) - 1);
    int hStartLimit = ((lineSize / 2) - (hSize / 2) + hOffset);
    int hStartAt = 64 - ((hBoss + 64) - ((lineSize / 2) - (hSize / 2) + hOffset));
    int hEndAt = hSize - (hBoss - hStartLimit) > 64 ? 64 : hSize - (hBoss - hStartLimit) + 2;
    //déterminer si le boss est dans l'écran d'affichage
    if(vStartAt < int(bossA.size()) && vEndAt > 0)
    {
        vStartAt = vStartAt > int(bossA.size()) || vStartAt < 0 ? 0 : vStartAt;
        vEndAt = int(bossA.size()) > vEndAt ? vEndAt : int(bossA.size());
        if (hStartAt < 64 && hBoss < ((lineSize / 2) + (hSize / 2) + hOffset))
        {
            //afficher le boss si il n'est pas mort
            if (bossHealth > 0)
            {
                hStartAt = hStartAt > 64 || hStartAt < 0 ? 0 : hStartAt;
                for (int iLine = vStartAt; iLine < vEndAt; ++iLine)
                {
                    string newLine;
                    int notAnsiCounter = 0;
                    string line = bossA[iLine];
                    for (int iCol = 0; iCol < int(bossA[iLine].size()); ++iCol)
                    {
                        if (line[iCol] == ' ' || line[iCol] == '.')
                        {
                            ++notAnsiCounter;
                            if (notAnsiCounter < hEndAt)
                            {
                                if (notAnsiCounter >= hStartAt)
                                    newLine += line[iCol];
                            }
                        }
                        else newLine += line[iCol];
                    }
                    int hMove = hBoss - hOffset < bossHOffset ? 1 : hBoss - bossHOffset - hOffset;
                    moveCursor(vBoss + iLine - bossVOffset - vOffset, hMove);
                    cout << newLine;
                }
            }

            //determiner si le boss est en collision avec le Protazoid
            if (vEndAt - vStartAt >= vSize / 2 - protHeight / 2 + 1)
            {
                if ((hBoss - hStartLimit) - hSize / 2 < protWidth / 2 && ((hBoss - hStartLimit) - (hSize / 2 - 64)) > -(protWidth / 2 - 1))
                {
                    if (bossHealth > 0 && protLife > 0)
                    {
                        moveCursor(vSize / 2 - protHeight / 2 - 2, hSize / 2 - protWidth / 2);
                        int protAttack = (protStrength + 1) + rand() % 6;
                        int bossAttack = (bossStrength + 1) + rand() % 6;
                        //si l'attaque du Protazoid est réussie, ajuster la santé du boss et afficher un message
                        if (protAttack >= bossAttack)
                        {
                            bossHealth -= 1;
                            string hit = string("\x1b[48;5;11m\x1b[38;5;16m") + "Bang!! (" + to_string(bossHealth) + ")";
                            string win = string("\x1b[48;5;11m\x1b[38;5;16m") + "Miam!! (" + to_string(bossHealth) + ")";
                            string attackresult = bossHealth <= 0 ? win : hit;
                            moveCursor(vSize / 2 - protHeight / 2 - 4, hSize / 2 - protWidth / 2);
                            cout << spaceString(7);
                            moveCursor(vSize / 2 - protHeight / 2 - 4, hSize / 2 - protWidth / 2);
                            cout << attackresult;
                        }
                        //si l'attaque du Protazoid est ratée, ajuster les pv du prot et afficher un message
                        else
                        {
                            protLife -= 1;
                            moveCursor(vSize / 2 - protHeight / 2 - 4, hSize / 2 - protWidth / 2);
                            cout << spaceString(10);
                            moveCursor(vSize / 2 - protHeight / 2 - 4, hSize / 2 - protWidth / 2);
                            cout << "\x1b[48;5;160m\x1b[38;5;15m" << "Ouch!! (" << protLife << ")";
                        }
                        protRegen = false;
                    }

                    //si les pv du boss sont a 0 ou dessous, lancer l'animation de mort et le win screen
                    if (bossHealth < 0)
                    {
                        if(bossHealth >= -8)
                            playBossDeath(vSize, hSize, hStartAt, hEndAt, vStartAt, vEndAt, vBoss, hBoss, bossVOffset, bossHOffset, vOffset, hOffset, protHeight, protWidth, -(bossHealth));
                        --bossHealth; //utilisé comme compteur pour les frame de mort du boss
                    }
                    bossHealth = bossHealth == 0 ? -1 : bossHealth;
                }
            }
        }
    }
}


bool drawMap(char& nextMove, bool& userQuit, int vSize, int hSize, int& vOffset, int& hOffset, int lineSize, array<string, 200> currentMap, int protWidth, int protHeight, int& protLife, int protLifeMax, int protSpeed, int protStrength, bool& protRegen, array<int, 4>& protInvQty, bool& protInvOpen, array<int, 20>& monstersPosition, array<int, 10>& monstersHealth, array<int, 10> monstersStrength, array<int, 100>& blobsPosition, array<int, 50>& blobsSizes, int& bossHealth, int& bossSteps, array<int, 2>& bossPosition, int bossStrength, int protSteps, array<string, 10>& UItexts, array<int, 10>& UIsteps, array<int, 20>& UIpositions, int& suddenDeathCtr) //codé par Alexis
{
    //si l'usager appuis sur ESCAPE, confirmer qu'il veut quitter et exécuter le choix
    if (int(nextMove) == 27) {
        int longest = 0;
        string quitLine1 = "Quitter maintenant signifie la fin de votre espèce, j'espère que vous en êtes conscient.";
        longest = quitLine1.size() > longest ? quitLine1.size() : longest;
        string quitLine2 = "Si oui, merci d'avoir joué! Appuyer sur o suivi de Entrer pour confirmer.";
        longest = quitLine2.size() > longest ? quitLine2.size() : longest;
        int vPos = vSize / 2;
        int hLongest = hSize / 2 - longest / 2;
        moveCursor(vPos - 1, hLongest);
        cout << spaceString(longest);
        moveCursor(vPos - 1, hSize / 2 - quitLine1.size() / 2);
        cout << "\x1b[48;5;234m\x1b[38;5;11m" << quitLine1;
        moveCursor(vPos, hLongest);
        cout << spaceString(longest);
        moveCursor(vPos, hSize / 2 - quitLine2.size() / 2);
        cout << "\x1b[48;5;234m\x1b[38;5;11m" << quitLine2;
        char confirmQuit;
        cin >> confirmQuit;
        if (int(confirmQuit) == 'o')
        {
            userQuit = true;
            return 0;
        }
    }

    bool protInvWasOpen = protInvOpen;
    //si l'usager appuis sur une touche sauf i/I, placer la carte au bon affichage et afficher les acteurs
    if (nextMove != 'i' && nextMove != 'I')
    {
        int firstLine = ((int(currentMap.size()) / 2) - int((vSize) / 2));
        vOffset += nextMove == 'w' ? -(protSpeed) : (nextMove == 's' ? protSpeed : 0);
        hOffset += nextMove == 'a' ? -(protSpeed*1.5) : (nextMove == 'd' ? protSpeed*1.5 : 0);
        moveCursor(1, 1);
        cout << "\x1b[48;5;234m" << "\x1b[38;5;76m";  //Ici pour mettre couleur carte et le reste du jeux.

        //affichage de la carte ligne par ligne
        for (int imap = ((int(currentMap.size()) / 2) - (vSize / 2)) + vOffset; imap < ((int(currentMap.size()) / 2) + (vSize / 2)) + vOffset; ++imap)
        {
            string line;
            if (imap < 0 || imap >= int(currentMap.size()) - 1)
                line = spaceString(lineSize);
            else
                line = currentMap[imap];
            string newLine;
            for (int iLine = 0; iLine < hSize; ++iLine)
            {
                if (((int(line.size()) / 2) - (hSize / 2) + iLine + hOffset) < 0 || ((int(line.size()) / 2) - (hSize / 2) + iLine + hOffset) > int(line.size()) - 1)
                    newLine += ' ';
                else
                    newLine += line[(int(line.size()) / 2) - (hSize / 2) + iLine + hOffset];
            }
            cout << newLine << endl;
        }
    }

    //si le Protazoid est hors de la carte, afficher un message d'avertissement et démarrer un compteur de mort subite
    if ((vOffset < -(int(currentMap.size()) / 2) || vOffset >(int(currentMap.size()) / 2) || hOffset < -(lineSize / 4) || hOffset >(lineSize / 4)) && protLife > 0)
        suddenDeath(vSize, hSize, suddenDeathCtr, UItexts, UIsteps, UIpositions, protHeight, protWidth);
    else
        suddenDeathCtr = 50;
    if (suddenDeathCtr <= 0) protLife = 0;

    if(rand() % 5 == 4) addBlobs(lineSize, currentMap, blobsPosition, blobsSizes); //ajoute des blobs aléatoirement
    //affichage des différents acteurs et ui, déplacements des monstres
    drawBlobs(vSize, hSize, vOffset, hOffset, lineSize, currentMap, protWidth, protHeight, protInvQty, blobsPosition, blobsSizes, protSteps, UItexts, UIsteps, UIpositions);
    moveMonsters(vOffset, hOffset, lineSize, currentMap, monstersPosition, monstersHealth);
    drawMonsters(vSize, hSize, vOffset, hOffset, lineSize, currentMap, protWidth, protHeight, protLife, protRegen, protStrength, protInvQty, monstersPosition, monstersHealth, monstersStrength);
    displayUI(UItexts, UIsteps, UIpositions, protLife, bossHealth); //affichage des differents textes du Protazoid

    //changer la condition pour changer le spawn du boss
    //  if (protLifeMax + protSpeed + protStrength >= 18) //8 évolutions(+10 de le creation du protazoid) du prot sont nécéssaires pour faire apparaitre le boss
    if (protSteps >= 500) //le boss apparait après 500 pas du Protazoid
    {
        if (protSteps == 500)
        {
            //affichage d'un message avertissement l'arrivée du boss
            int vPos = vSize / 2 - protHeight / 2;
            string uiString = string("\x1b[48;5;11m\x1b[38;5;16m") + "Un grondement effroyable se fait sentir dans toute la soupe!!";
            int hPos = hSize / 2 - int(uiString.size()) / 2;
            addTextToUI(vPos, hPos, uiString, UItexts, UIsteps, UIpositions, 9);
        }
        //si le boss est encore vivant, le déplacer et l'Afficher
        if(bossHealth > 0)
            moveBoss(vOffset, hOffset, lineSize, currentMap, bossSteps, bossPosition);
        drawBoss(vSize, hSize, vOffset, hOffset, lineSize, currentMap, protWidth, protHeight, protLife, protRegen, protStrength, bossPosition, bossHealth, bossStrength);
    }

    //affichage de l'inventaire ligne par ligne et affichage des valeurs et évolution en cours
    int vInvTop = 12;
    int hInvLeft = 16;
    if (nextMove == 'i' || nextMove == 'I' || protInvOpen)
    {
        for (int i = 0; i < int(Inventory.size()); ++i)
        {
            moveCursor(vInvTop + i, hInvLeft);
            cout << Inventory[i];
        }
        moveCursor(vInvTop + 7, hInvLeft + 25);
        cout << protLife << " / " << protLifeMax;
        moveCursor(vInvTop + 9, hInvLeft + 25);
        cout << protSpeed;
        moveCursor(vInvTop + 11, hInvLeft + 25);
        cout << protStrength;
        moveCursor(vInvTop + 13, hInvLeft + 25);
        cout << protInvQty[0];
        string evoluting;
        int evolutionETA = 0;
        if (protInvQty[1] > 0)
        {
            evoluting = "Vie";
            evolutionETA = protInvQty[1];
        }
        if (protInvQty[2] > 0)
        {
            evoluting = "Force";
            evolutionETA = protInvQty[2];
        }
        if (protInvQty[3] > 0)
        {
            evoluting = "Vitesse";
            evolutionETA = protInvQty[3];
        }
        if (evolutionETA != 0)
        {
            moveCursor(vInvTop + 7, hInvLeft + 25 + 29 - evoluting.size());
            cout << evoluting;
            string evoCalories = to_string(evolutionETA) + " calories à trouver";
            moveCursor(vInvTop + 8, hInvLeft + 17 + evoCalories.size());
            cout << evoCalories;
        }
        protInvOpen = true;
    }
    //si l'usager fait un choix mais n'a pas assez de calories
    if ((nextMove == '1' || nextMove == '2' || nextMove == '3') && protInvOpen && protInvQty[0] < 5)
    {
        moveCursor(vInvTop + 18, hInvLeft + 1);
        cout << "Vous n'avez pas assez de calories pour acheter une évolution.";
    }
    //si l'usager fait un choix et a assez de calories, confirmer et ajouter à l'inventaire
    if ((nextMove == '1' || nextMove == '2' || nextMove == '3') && protInvOpen && protInvQty[0] >= 5)
    {
        moveCursor(vInvTop + 18, hInvLeft + 4);
        cout << "Voulez-vous achetez une évolution de ";
        if (nextMove == '1')
        {
            cout << "Vie?";
            moveCursor(vInvTop + 19, hInvLeft + 4);
            cout << "Appuyez sur o pour oui suivi de la touche Entrer.";
        }
        if (nextMove == '2')
        {
            cout << "Vitesse?";
            moveCursor(vInvTop + 19, hInvLeft + 4);
            cout << "Appuyez sur o pour oui suivi de la touche Entrer.";
        }
        if (nextMove == '3')
        {
            cout << "Force?";
            moveCursor(vInvTop + 19, hInvLeft + 4);
            cout << "Appuyez sur o pour oui suivi de la touche Entrer.";
        }
        char buyValidate;
        cin >> buyValidate;
        if (buyValidate == 'o' || buyValidate == 'O')
        {
            protInvQty[0] -= 5;
            if (nextMove == '1')
                protInvQty[1] += 10*protLifeMax;
            if (nextMove == '2')
                protInvQty[3] += 10*protSpeed;
            if (nextMove == '3')
                protInvQty[2] += 10*protStrength;

            char refreshMap = 'r';
            drawMap(refreshMap, userQuit, vSize, hSize, vOffset, hOffset, lineSize, currentMap, protWidth, protHeight, protLife, protLifeMax, protSpeed, protStrength, protRegen, protInvQty, protInvOpen, monstersPosition, monstersHealth, monstersStrength, blobsPosition, blobsSizes, bossHealth, bossSteps, bossPosition, bossStrength, protSteps, UItexts, UIsteps, UIpositions, suddenDeathCtr);
        }
        else
            protInvOpen = false;
    }

    //close Inventory if the user moves
    if ((nextMove == 'w' || nextMove == 'a' || nextMove == 's' || nextMove == 'd') && protInvOpen)
    {
        protInvOpen = false;
    }
    if (!protInvOpen && protInvWasOpen)
    {
        char refreshMap= 'r';
        drawMap(refreshMap, userQuit, vSize, hSize, vOffset, hOffset, lineSize, currentMap, protWidth, protHeight, protLife, protLifeMax, protSpeed, protStrength, protRegen, protInvQty, protInvOpen, monstersPosition, monstersHealth, monstersStrength, blobsPosition, blobsSizes, bossHealth, bossSteps, bossPosition, bossStrength, protSteps, UItexts, UIsteps, UIpositions, suddenDeathCtr);
    }
    return true;
}

int main()
{
    
#if _WIN32 || _WIN64
    system("chcp 65001");
    system("cls");
#endif
    srand(time(0));
    
    //variables utilisées pour la carte
    bool userQuit = false;
    int hSize = 200;
    int vSize = 60;
    int vOffset = 0;
    int hOffset = 0;
    int lineSize = 800;
    array<string, 200> currentMap;
    int suddenDeathCtr = 50;

    //calibration de l'écran d'affichage et variables d'affiache
    calibrateScreen(vSize, hSize);
    array<string, 10> UItexts = {};
    array<int, 10> UIsteps = {};
    array<int, 20> UIpositions = {};

    //variables utilisées pour le Protazoid
    int protSteps = 0;
    string protName;
    int protWidth = 16;
    int protHeight = 11;
    int protLife;
    int protLifeMax;
    bool protRegen;
    int protSpeed;
    int protStrength;
    int protSatiety = 100;
    array<int, 4> protInvQty = { 0, 0, 0, 0 };
    bool protInvOpen = false;
    char nextMove;

    //variables utilisées par les monstres
    array<int, 20> monstersPosition;
    array<int, 10> monstersHealth;
    array<int, 10> monstersStrength;

    //variables utilisées par les blobs
    int blobsNumber = 25 + (rand() % 26);
    array<int, 100> blobsPosition;
    array<int, 50> blobsSizes;

    //variables utilisées par le boss
    int bossSteps = 0;
    array<int, 2> bossPosition = { 20 , 360 };
    int bossHealth = 15;
    int bossSpeed = 1;
    int bossStrength = 10;

    string startString = "Voulez-vous débuter un nouveau Protazoid? o pour débuter.";
    int longestString = int(startString.size());

    cout << "Appuyez sur S pour skipper l'intro ou une autre touche pour le voir et appuyez sur Entrée.\n";
    bool skipIntro = false;
    char userSkip;
    cin >> userSkip;
    clearCin();
    if (userSkip == 'S' || userSkip == 's')
        skipIntro = true;

    //jouer l'intro si la taille de l'écran est d'au moins 200 x 60
    if(vSize >= 60 && hSize >= 200)
        displayIntro(50, skipIntro, vSize, hSize);

    bool startGame = displayStartGame(vSize, hSize, longestString);

    //BOUCLE DE JEU PRINCIPALE
    while (startGame)
    {
        //réinitialisation des variables
        //variables utilisées pour la carte
        userQuit = false;
        vOffset = 0;
        hOffset = 0;
        suddenDeathCtr = 50;
        //variables utilisées pour le protazoid
        protSteps = 0;
        protWidth = 16;
        protHeight = 11;
        protSatiety = 100;
        //array<string, 4> protInvNames = { "Calories", "Life", "Speed", "Strength" };
        protInvQty = { 0, 0, 0, 0 };
        protInvOpen = false;
        //variable utilisés par les monstres
        monstersPosition = {};
        monstersHealth = {};
        monstersStrength = {};
        //variables utilisées par les blobs
        blobsNumber = 25 + (rand() % 26);
        blobsPosition = {};
        blobsSizes = {};
        //variables utilisées par le boss
        bossSteps = 0;
        bossPosition = { 20 , 360 };
        bossHealth = 15;
        bossSpeed = 1;
        bossStrength = 10;

        //positionnement et initialisation des acteurs
        currentMap = buildRandomMap(lineSize);
        positionBlobs(lineSize, currentMap, blobsNumber, blobsPosition);
        sizeBlobs(blobsNumber, blobsSizes);
        positionMonsters(lineSize, currentMap, monstersPosition, monstersHealth, monstersStrength);

        //création du personnage, choix: mouvement, points de vie, force; plus tard: type du protazoid
        //4 points a répartir comme on veut entre les 3 choix
        //minimum de 2 dans chacune des caractéristiques, 1 point équivaut à 1 de plus dans la caractéristique

        //valide le nom du Protazoid
        startNameValidation(vSize, hSize, protName, longestString);

        //boucle pour que l'usager valide les points de caractéristiques de son protazoid
        if(protName != "godmode")
            validateCaracteristics(protLife, protLifeMax, protSpeed, protStrength, protName, vSize, hSize);
        else //si le nom du prot est godmode, mettre les caracs au maximum
        {
            protLife = 10;
            protLifeMax = 10;
            protSpeed = 10;
            protStrength = 10;
        }

        //démarrage de la partie
        system("cls");
        bool inMap = true;
        while (inMap)
        {
            protRegen = true;
            drawMap(nextMove, userQuit, vSize, hSize, vOffset, hOffset, lineSize, currentMap, protWidth, protHeight, protLife, protLifeMax, protSpeed, protStrength, protRegen, protInvQty, protInvOpen, monstersPosition, monstersHealth, monstersStrength, blobsPosition, blobsSizes, bossHealth, bossSteps, bossPosition, bossStrength, protSteps, UItexts, UIsteps, UIpositions, suddenDeathCtr);
            if (userQuit) break;
            inMap = drawProtazoid(vSize, hSize, protWidth, protHeight, protLife, protLifeMax, protRegen, protSpeed, protStrength, protSatiety, protInvQty, UItexts, UIsteps, UIpositions);
            if (protLife > 0 && bossHealth > 0)
            {
                cout << ESC + (to_string(vSize + 1) + ";1H");
                cout << "\x1b[48;5;234m\x1b[38;5;11m" << "w,a,s,d pour faire vous déplacer, i pour ouvrir l'inventaire. Pour quitter, appuyez sur la touche ESCAPE." << "\x1b[0m\x1b[48;5;234m";
                nextMove = waitKeyToPress();
                ++protSteps;
            }
            else if(protLife <= 0 && protLife > -9 || bossHealth <= 0 && bossHealth > -9)
            {
                nextMove = 'r';
                sleep_for(250ms);
            }
            //si l'animation de mort du Protazoid est terminée, afficher l'écran de fin de jeu
            if (protLife <= -9)
            {
                char restart;
                string gameOverLine1 = "Vous êtes mort.";
                string gameOverLine2 = "L'histoire de votre espèce se termine ici, meilleure chance à la prochaine incarnation.";
                string gameOverLine3 = "Appuyez sur q pour quitter Protozoa ou r pour renaître suivi de Entrer.";
                moveCursor(vSize / 2 - 2, hSize / 2 - int(gameOverLine1.size()) / 2);
                cout << "\x1b[48;5;234m\x1b[38;5;11m" << gameOverLine1;
                sleep_for(1000ms);
                moveCursor(vSize / 2 - 2 + 1, hSize / 2 - int(gameOverLine2.size()) / 2);
                cout << "\x1b[48;5;234m\x1b[38;5;11m" << gameOverLine2;
                sleep_for(1000ms);
                moveCursor(vSize / 2 - 2 + 2, hSize / 2 - int(gameOverLine3.size()) / 2);
                cout << "\x1b[48;5;234m\x1b[38;5;11m" << gameOverLine3;
                sleep_for(1000ms);
                cin >> restart;
                if (restart == 'q' || restart == 'Q')
                {
                    inMap = false;
                    startGame = false;
                }
                if (restart == 'r' || restart == 'R')
                {
                    inMap = false;
                }

            }
            //si l'animation de mort du boss est terminé, afficher l'écran de victoire et de fin du jeu
            if (bossHealth <= -12)
            {
                char restart;
                string gameOverLine1 = "Vous avez gagné, " + protName + "!!";
                string gameOverLine2 = "Vos descendant ne se rappelleront pas de vous mais sachez que vous avez été primordial dans leur évolution.";
                string gameOverLine3 = "Appuyez sur q pour quitter Protozoa ou r pour renaître suivi de Entrer.";
                moveCursor(vSize / 2 - 2, hSize / 2 - int(gameOverLine1.size()) / 2);
                cout << "\x1b[48;5;234m\x1b[38;5;11m" << gameOverLine1;
                sleep_for(1000ms);
                moveCursor(vSize / 2 - 2 + 1, hSize / 2 - int(gameOverLine2.size()) / 2);
                cout << "\x1b[48;5;234m\x1b[38;5;11m" << gameOverLine2;
                sleep_for(1000ms);
                moveCursor(vSize / 2 - 2 + 2, hSize / 2 - int(gameOverLine3.size()) / 2);
                cout << "\x1b[48;5;234m\x1b[38;5;11m" << gameOverLine3;
                sleep_for(1000ms);
                cin >> restart;
                if (restart == 'q' || restart == 'Q')
                {
                    inMap = false;
                    startGame = false;
                }
                if (restart == 'r' || restart == 'R')
                {
                    inMap = false;
                }
            }
        }
        if (userQuit) break;
    }    
    return 0;
}