#include "qwarlockdictionary.h"


void QWarlockDictionary::fillGameDictionary() {
    qDebug() << "QWarlockDictionary::fillGameDictionary";

    // fill dictionary
    fillDictionary("AreYouSure", "Ви впевненні?", "Вы уверенны?", "Are you sure?");
    fillDictionary("NoGestureForTurn", "Ви не задали жестів на цей хід!", "Вы не задали жесты для хода!", "You have no set gesture for this turn!");
    fillDictionary("SurrenderForTurn", "Ви хочете здатись?", "Вы хотите сдаться?", "Are you really want to surrender?");
    fillDictionary("ConfirmOrdersForTurn", "", "", "Are you confirm your orders?");
    fillDictionary("ConfirmLogout" ,"" , "", "Are you confirm your decision?");
    fillDictionary("LeftHand", "Ліва рука", "Левая рука", "Left Hand");
    fillDictionary("RightHand", "Права рука", "Правая рука", "Right Hand");
    fillDictionary("LH", "Ліва рука", "Левая рука", "Left Hand");
    fillDictionary("RH", "Права рука", "Правая рука", "Right Hand");
    fillDictionary("Submit", "Походити", "Походить", "Submit");
    fillDictionary("DelaySpell", "Зберегти закляття", "Сохранить заклятие", "Delay Spell");
    fillDictionary("Neither", "Зберегти закляття", "Сохранить заклятие", "Neither");
    fillDictionary("Left", "Лівої руки", "Левой руки", "Left");
    fillDictionary("Right", "Правої руки", "Правой руки", "Right");
    fillDictionary("Two-handed", "Двуручне", "Двухручное", "Two-handed");
    fillDictionary("MakeSpellPermanent", "Зробити закляття постійним", "Сделать проклятие посчтоянным", "Make spell permanent");
    fillDictionary("ChatMessage", "Повідомлення в чат", "Сообщение в чат", "Chat message");
    fillDictionary("MonsterFrom", "монстр з", "монстр с", "monster from");
    fillDictionary("EnterLoginPass", "Введіть логін та пароль, будь ласка", "Введите логин и пароль, пожалуйста", "Please enter Login and Password");
    fillDictionary("Apply", "OK", "OK", "Apply");
    fillDictionary("OK", "OK", "OK", "OK");
    fillDictionary("Cancel", "Відміна", "Отмена", "Cancel");
    fillDictionary("EnterProxy", "Введіть налаштування проксі", "Введите настройки прокси", "Enter proxy settigns");
    fillDictionary("ProxyHost", "Проксі адреса", "Прокси адрес", "Proxy host");
    fillDictionary("ProxyPort", "Проксі порт", "Прокси порт", "Proxy port");
    fillDictionary("ProxyUser", "Проксі користувач", "Прокси пользователь", "Proxy user");
    fillDictionary("ProxyPass", "Проксі пароль", "Прокси пароль", "Proxy password");
    fillDictionary("Close", "Закрити", "Закрыть", "Close");
    fillDictionary("WatchOnline", "Переглянути онлайн", "Смотреть онлайн", "Watch in browser");
    fillDictionary("Settings", "Налаштування", "Настройки", "Settings");
    fillDictionary("LoginSett", "Налаштування входу", "Настройки входа", "Login settings");
    fillDictionary("NetworkSett", "Налаштування підключення", "Настройки сети", "Network settings");
    fillDictionary("LangSett", "Налаштування мови", "Настройки языка", "Language settings");
    fillDictionary("Exit", "Вихід", "Выход", "Exit");
    fillDictionary("AddChallengeTitle", "Ваша нова битва", "Ваша новая битва", "Your own new battle");
    fillDictionary("AddChallenge", "Додати поєдинок", "Добавить поединок", "Add Challenge");
    fillDictionary("EnterChallenge", "Налаштування дуелі", "Настройки дуели", "Please enter Challenge settings");
    fillDictionary("WarlocksCount", "Кількість учасників", "Количество участников", "WarlocksCount");
    fillDictionary("FriendlyLevel", "Рівень поєдинку", "Уровень поединка", "Friendly level");
    fillDictionary("Ladder", "Турнірний", "Турнирный", "Ladder");
    fillDictionary("Friendly", "Дружній", "Дружественный", "Friendly");
    fillDictionary("VFriendly", "Тренувальний", "Тренировочный", "Very friendly");
    fillDictionary("Description", "Декілька слів опонентам", "Несколько слов противникам", "Few word for opponents");
    fillDictionary("EnterNewUser", "Заповніть дані нового чаклуна", "Введите данные нового колдуна", "Enter new warlock data");
    fillDictionary("Login", "Логін", "Логин", "Login");
    fillDictionary("Password", "Пароль", "Пароль", "Password");
    fillDictionary("Email", "Email", "Email", "Email");
    fillDictionary("Im13", "Мені як мінімум 13", "Мне как минимум 13", "I am a least 13 years old");
    fillDictionary("Register", "Зареєструвати", "Зарегистрировать", "Register");
    fillDictionary("LoginIn", "Зайти", "Войти", "Login in");
    fillDictionary("LangUA", "Українська", "Українська", "Українська");
    fillDictionary("LangEN", "English", "English", "English");
    fillDictionary("LangRU", "Русский", "Русский", "Русский");
    fillDictionary("Gestures", "Жести", "Жесты", "Gestures");
    fillDictionary("Hint", "Підказка", "Подсказка", "Hint");
    fillDictionary("SpellName", "Назва закляття", "Название заклинания", "Spell name");
    fillDictionary("Warlocks", "Чаклуни", "Колдуны", "Warlocks");
    fillDictionary("Fast", "Швидкий", "Быстрый", "Fast");
    fillDictionary("Level", "Рівень", "Уровень", "Level");
    fillDictionary("ParaFC", "ParaFC", "ParaFC", "ParaFC");
    fillDictionary("Maladroit", "Maladroit", "Maladroit", "Maladroit");
    fillDictionary("Description", "Опис", "Описание", "Description");
    fillDictionary("Accept", "Прийняти", "Принять", "Accept");
    fillDictionary("Melee", "Бугурт", "Толпа", "Melee");
    fillDictionary("NeedMore", " Потрібно ще %1", " Надо еще %1", " Need %1 more");
    fillDictionary("Played", "Зіграно", "Сыграно", "Played");
    fillDictionary("Won", "Перемог", "Выиграшей", "Won");
    fillDictionary("Died", "Смертей", "Умер", "Died");
    fillDictionary("LadderScore", "Турнірний рівень", "Турнирный уровень", "Ladder Score");
    fillDictionary("MeleeScore", "Бугуртний рівень", "Бугуртный уровень", "Melee Score");
    fillDictionary("Elo", "Рейтинг", "Рейтинг", "Elo");
    fillDictionary("FinishB", "Завершені поєдинки", "Законченые бои", "Finished battles");
    fillDictionary("WaitB", "Незавершені поєдинки", "Незаконченые бои", "Waiting in battles");
    fillDictionary("ReadyB", "Поєдинки, що чекають дій", "Бои которые ожидают действий", "Waiting action");
    fillDictionary("ChallengeB", "Виклики", "Вызовы", "Challenges");
    fillDictionary("BattleList", "Поєдинки, до яких можна приєднатись", "Бои к которым можно присоединиться", "Battle list");
    fillDictionary("TopList", "Чемпіони", "Чемпионы", "Hall of Fame");
    fillDictionary("Messages", "Повідомлення", "Сообщения", "Messages");
    fillDictionary("SpellList", "Список доступних заклять", "Список доступніх заклинаний", "Spell list");
    fillDictionary("Accounts", "Акаунти", "Акаунты", "Accounts");
    fillDictionary("Iam13", "Мені виповнилось 13", "Мне исполнилось 13", "I am 13+ years old");
    fillDictionary("ConfirmExit", "Закрити гру?", "Закрыть игру?", "Leave game?");
    fillDictionary("WndErrorTitle", "Помилка!", "Ошибка!", "Error!");
    fillDictionary("WaitTrainingBattle", "Ви зможете створювати дуелі, після завершення тренувального бою, почекайте будь ласка", "Вы сможете создавать дуэли, после завершения тренировочного боя, подождите пожалуйста", "You will be able to create duels, after the end of a training battle, please wait");
    fillDictionary("JustRegistered", "Вітаю нового чарівника! Зачекайте будь ласка скоро розпочнеться ваш перший тренувальний бій.<br>В бою ви будете складати закляття роблячи жести вашими руками. Всього є 6 жестів W,S,D,P,C,F<br>За один раунд бою ви можете зробити 1 жест лівою та 1 жест правою рукою. В наступному раунді ви продовжите ланцюжок женстів для створення закляття лівою та правою рукою. Наприклад в першому раунді ви зробили жест лівою рукою S та правою D, в наступному раунді роблячи лівою рукою жест D ви завершаєте закляття Магічної стріли (SD) а правою можете продовжити побудову закляття Заплутаність (DSF) роблячи жест S. Весь список заклять є в книзі заклятть.",
                                     "Приветствую нового волшебника! Пожалуйста, подождите скоро начнется ваш первый тренировочный бой. <br> В бою вы будете составлять заклинания делая жесты вашими руками. Всего есть 6 жестов W, S, D, P, C, F <br> За один раунд боя вы можете сделать 1 жест левой и 1 жест правой рукой. В следующем раунде вы продолжите цепочку женстей для создания заклинания левой и правой рукой. Например, в первом раунде вы сделали жест левой рукой S и правой D, в следующем раунде делая левой рукой жест D вы завершают заклятие Магической стрелы (SD), а правой можете продолжить построение заклятие Запутанность (DSF) делая жест S. Полный список заклинаний есть в книге заклинаний.",
                                     "Hey #login#, let's fight!<br>Check your spell book, and learn how to cast spells.<br>Just pick a gesture for each of your hands.<br>Gestures are letters W, S, D, P, C, F<br>Letters add up to words (like DPP), and words are spells (like Amnesia).<br>Let's go!");
    fillDictionary("NewGame", "Нова гра", "Новая игра", "New Game");
    fillDictionary("TrainingGame", "", "", "Start Training");
    fillDictionary("NewGameWithBot", "Грати з ботом", "Играть с ботом", "Start Training");
    fillDictionary("NewGameWithPlayer", "Грати з людиною", "Играть с человеком", "Warlock vs. Warlock");
    fillDictionary("ActiveGameTitle", "", "", "Your Games:");
    fillDictionary("FinishedGameTitle", "", "", "Completed Games:");
    fillDictionary("MonsterSetTarget", "", "", "Choose target");
    fillDictionary("MonsterOwnerTitle", "", "", "Owner");
    fillDictionary("MonsterTargetTitle", "", "", "Target");
    fillDictionary("ReviewOrders", "", "", "Review your orders");
    fillDictionary("SendOrders", "", "", "Submit");
    fillDictionary("TitleAction_charmed", "", "", "Choose gesture for Charmed warlock");
    fillDictionary("TitleAction_paralized", "", "", "Choose paralyzed hand");
    fillDictionary("TitleAction_D", "", "", "Try to bank the spell cast by ");
    fillDictionary("TitleAction_P", "", "", "Try do permanent spell cast by ");
    fillDictionary("TitleAction_CP", "", "", "Paralyze %1 for ");
    fillDictionary("TitleAction_CC", "", "", "Charm %1 to do %2 for ");
    fillDictionary("TitleAction_p", "", "", "SURRENDERING (P/P) - raising a white flag");
    fillDictionary("TitleAction_M", "", "", "direct %1<br>at %2");
    fillDictionary("TitleActionHint_pp", "", "", "P/P means surrender. Is that what you want?");
    fillDictionary("TitleActionHint_nobody", "", "", "Really? You are targeting 'nobody'");
    fillDictionary("TitleActionHint_knifes", "", "", "You only have 1 knife, and right hand goes first");
    fillDictionary("TitleActionHint_monster1", "", "", "Do you really want the monster to attack you?");
    fillDictionary("TitleActionHint_monster2", "", "", "Do you really want to target your own monster?");
    fillDictionary("TitleActionHint_summon", "", "", "Really? Your summon will serve your opponent");
    fillDictionary("TitleActionHint_clap", "", "", "Really? No spell has 'c' only one one hand");

    fillDictionary("Refresh", "", "", "Refresh");
    fillDictionary("DialogStartBattle", "", "", "Start battle #ID #FL with #LOGINS");
    fillDictionary("DialogJoinBattle", "", "", "Join battle #ID #FL with #LOGINS need #NEED more except you");
    fillDictionary("DialogJoinBattle", "", "", "Join battle #ID #FL with #LOGINS need #NEED more except you");
    fillDictionary("LoginAs", "", "", "Login as ...");
    fillDictionary("SpellbookActionTitle", "", "", "Spellbook");
    fillDictionary("SpellbookTitle", "", "", "Spellbook Level: ");
    fillDictionary("SpellbookSubTitle", "", "", "Note: Small caps like 'c' indicate gestures that need to be performed with both hands");
    fillDictionary("SpellbookHint1", "", "", "Stay alive for at least 15 turns to unlock spellbook level 2!");
    fillDictionary("SpellbookHint2", "", "", "Win a training game to unlock spellbook level 3!");
    fillDictionary("SpellbookHint3", "", "", "Win 2 more training games to unlock spellbook level 4!");
    fillDictionary("SpellbookHint3.1", "", "", "Win 1 more training games to unlock spellbook level 4!");
    fillDictionary("SpellbookHint4", "", "", "Survive 15 turns or win in a warlock vs. warlock game to unlock the Master Warlock Spellbook!");
    fillDictionary("SpellbookName1", "", "", "Spellbook: Apprentice Level 1");
    fillDictionary("SpellbookName2", "", "", "Spellbook: Apprentice Level 2");
    fillDictionary("SpellbookName3", "", "", "Spellbook: Apprentice Level 3");
    fillDictionary("SpellbookName4", "", "", "Spellbook: Warlock Level 4");
    fillDictionary("SpellbookName5", "", "", "Master Warlock Spellbook");
    fillDictionary("Warlock", "", "", "Warlock");
    fillDictionary("RH_summon", "", "", "Right Hand Monster");
    fillDictionary("LH_summon", "", "", "Left Hand Monster");
    fillDictionary("WH_summon", "", "", "Monster that warlock may be summoning");
    fillDictionary("DefErrTitle", "", "", "Something is wrong...");
    fillDictionary("BattleCreateError", "", "", "The battle couldn't be created");
    fillDictionary("JoinBattleError", "", "", "Can't join battle");
    fillDictionary("BattleCreateErrorDetails", "", "", "You may only play 3 games at any given time.<br><br>Note: This open source project was build based on Ravenblack Games services (whicj are not affiliated with this android app)<br><br>You may register with Ravenblack Games to increase the number of concurrent games to 20.");
    fillDictionary("LearnMore", "", "", "Learn More");
    fillDictionary("DefNotStartTitle", "", "", "Waiting to Start...");
    fillDictionary("DefNotStartSTitle", "", "", "The battle has not yet begun.");
    fillDictionary("DefNotStartDesc", "", "", "Training games may take a few minutes to start. In games against other warlocks it may take a few hours until another warlock joins your game.");
    fillDictionary("SpellGroup0", "", "", "Undefined");
    fillDictionary("SpellGroup1", "", "", "Disruptions");
    fillDictionary("SpellGroup2", "", "", "Damaging");
    fillDictionary("SpellGroup3", "", "", "Healing");
    fillDictionary("SpellGroup4", "", "", "Monsters");
    fillDictionary("SpellGroup5", "", "", "Protection");
    fillDictionary("SpellGroup6", "", "", "Counters");
    fillDictionary("SpellGroup7", "", "", "Advanced Enchantments");
    fillDictionary("SpellGroup8", "", "", "Elements");
    fillDictionary("NewGameWithFriend", "", "", "Duel with a Friend");
    fillDictionary("FnNotReadyTitle", "", "", "Coming Soon");
    fillDictionary("FnNotReady_short_desc", "", "", "Hey! this is not ready yet, wanna help?");
    fillDictionary("FnNotReady_desc", "", "", "Warlock's Duel is a free open source game, developed by the Spellcaster community. You can give us a hand, or just chat with like-minded people");
    fillDictionary("JoinUs", "", "", "Join us");
    fillDictionary("Feedback", "", "", "Feedback");
    fillDictionary("FeedbackTitle", "", "", "We need your help :)");
    fillDictionary("Feedback_short_desc", "", "", "We're putting a lot of love to bring this old (1977) favorite game back to life.<br><br>Would you help us improve by answering 3 quick questions?");
    fillDictionary("RateUs", "", "", "Rate Us");
    fillDictionary("RateUsAction", "", "", "Rate");
    fillDictionary("SendFeedback", "", "", "Send Feedback");
    fillDictionary("RateUs_short_desc", "", "", "This is an open source project, and we're doing our best to bring this old (1977) favorite game back to life.<br><br>Would you help us with a 5 star rating?");
    fillDictionary("miRulesTitle", "", "", "Advanced Rules");
    fillDictionary("miLogoutTitle", "", "", "Sign Out");
    fillDictionary("BattleHistoryTitle", "", "", "Battle History");
    fillDictionary("Say", "", "", "Add a message");
    fillDictionary("PersonalChalengeTitle", "", "", "Private Duel Invite");
    fillDictionary("PersonalChalengeSDesc", "", "", "You are invited for a private duel by ");
    fillDictionary("OpenChalengeTitle", "", "", "Open duel invite");
    fillDictionary("OpenChalengeSDesc", "", "", "You are invited for a open duel by ");
    fillDictionary("NewChalengeSDesc", "", "", "New Match: ");
    fillDictionary("PersonalChalengeAccept", "", "", "Accept");
    fillDictionary("PersonalChalengeReject", "", "", "Reject");
    fillDictionary("DuelWithFriend", "", "", "Duel with a friend");
    fillDictionary("ShareWndTitle", "", "", "Invite friends");
    fillDictionary("ShareWndSDesc", "", "", "Please share following link with your friends");
    fillDictionary("ShareWndDesc", "", "", "Link should be in your clipboad already<br><br>");
    fillDictionary("SearchWizardTitle", "", "", "Start a Private duel");
    fillDictionary("SearchWizardShortDesc", "", "", "Find a warlock by name and invite for a private duel");
    fillDictionary("SearchWizardInputTitle", "", "", "Invite a warlock by name:");
    fillDictionary("NewGameWithPlayerTitle", "", "", "Choose your partner");
    fillDictionary("NewGameWithPlayerSDesc", "", "", "Start a duel against a random warlock or invite a friend to play");
    fillDictionary("NewGameWithPlayerBtn1", "", "", "Warlock");
    fillDictionary("NewGameWithPlayerBtn2", "", "", "Friend");
    fillDictionary("NewGameWithPlayerBtn3", "", "", "Find a warlock by name");
    fillDictionary("NewGameFindByName", "", "", "Find by name");
    fillDictionary("NewGameWithBotTitle", "", "", "Choose your training partner");
    fillDictionary("NewGameWithBotSDesc", "", "", "Play against an AI training bot, or invite a friend and train against each other");
    fillDictionary("NewGameWithBotBtn1", "", "", "Training bot");
    fillDictionary("NewGameWithBotBtn2", "", "", "Friend");
    fillDictionary("NewGameWithBotBtn3", "", "", "Find a warlock by name");
    fillDictionary("NewVFGameBtn1", "", "", "Training bot");
    fillDictionary("NewVFGameBtn2", "", "", "Practice duel");
    fillDictionary("NewVFGameBtn3", "", "", "Invite a friend");
    fillDictionary("NewFGameBtn1", "", "", "Public");
    fillDictionary("NewFGameBtn2", "", "", "Private");
    fillDictionary("NewFGameBtn3", "", "", "Invite a friend");
    fillDictionary("TooMuchBotBatleWndTitle", "", "", "Training battle limit");
    fillDictionary("TooMuchBotBatleWndSDesc", "", "", "Please wait for your turn in current training battle");
    fillDictionary("FinishedGameResultTitleWin", "", "", "Yey! You won the game.");
    fillDictionary("FinishedGameResultTitleLose", "", "", "Uh Oh... You lost the game.");
    fillDictionary("FinishedGameResultTitleDraw", "", "", "Hmm... It's a draw.");
    fillDictionary("FinishedGameResultTitleUnknown", "", "", "Damn! What's going on?");
    fillDictionary("NewGameMoreOption", "", "", "More options");
    fillDictionary("NewGameAutoMatch", "", "", "Play");
    fillDictionary("NewGameDuel", "", "", "A New Duel:");
    fillDictionary("NewGamePractice", "", "", "Practice Mode:");


    fillDictionary("old_hint_1_1", "", "", "Every letter represents a gesture. A set of gestures like DPP is a spell (Amnesia)");
    fillDictionary("old_hint_1_2", "", "", "Some useful spells: SD (Magic Missile) deals 1 damage. P (shield) will defend against it...");
    fillDictionary("hint_1_1", "", "", "Hint: Start by picking the 'S' gesture with both hands");
    fillDictionary("hint_1_2", "", "", "Then in the next 2 turns cast Summon Gobline ('SFW')...");
    fillDictionary("hint_1_3", "", "", "...and Magic Missile ('SD')");
    fillDictionary("hint_1_4", "", "", "Be careful, P/P with both hands means surrender.");
    fillDictionary("hint_1_5", "", "", "Check your spellbook to learn more spells. Let's start!");
    fillDictionary("old_hint_1_1", "", "", "Hint: Pick the '>' gesture (Stab) to inflict 1 point of damage");
    fillDictionary("old_hint_1_2", "", "", "With your other hand, cast the simplest spell, 'P' (shield) to defend...");
    fillDictionary("old_hint_1_3", "", "", "...be careful: P/P with both hands means surrender. (marked as lowercase 'p' in your spellbook)");
    fillDictionary("old_hint_1_4", "", "", "Afterwards you can cast longer spells like Summon Goblin ('SFW'). This takes 3 turns to cast.");
    fillDictionary("old_hint_1_5", "", "", "Check your spellbook to learn more spells. Let's start!");
    fillDictionary("hint_2_1", "", "", "Hint: Disruptions like DPP (Amnesia) and SWD (Fear) disrupt your enemy...");
    fillDictionary("hint_2_2", "", "", "...but when two are cast at the same time they cancel out.");
    fillDictionary("hint_3_1", "", "", "Hint: When you summon a goblin (SFW), cast it at yourself, so it serves you.");
    fillDictionary("hint_3_2", "", "", "Afterwards, click the goblin to set its target.");
    fillDictionary("old_hint_3_3", "", "", "You can also inflict direct damage with Cause light or heavy wounds (WFP, WPFD)");
    fillDictionary("old_hint_3_4", "", "", "...and heal up with Cure (DFW, DFPW)");
    fillDictionary("hint_4_1", "", "", "Hint: There are two ways to counter a spell (WPP, WWS).");
    fillDictionary("hint_4_2", "", "", "Cast it at yourself to prevent spells and monsters from hitting you.");
    fillDictionary("hint_4_3", "", "", "Cast at an opponent to prevent them from summoning a monster.");
    fillDictionary("hint_5_1", "", "", "You're doing great! Keep up.");
    fillDictionary("hint_5_2", "", "", "Playing against other players and friends is fun...");
    fillDictionary("hint_5_3", "", "", "Games with other players are played in turns.");
    fillDictionary("hint_5_4", "", "", "You can play up-to 3 games at the same time, and you have up to 3 days to play each turn.");
    fillDictionary("old_hint_6_1", "", "", "Hint: When playing against real players, you'll be seeing a lot of these spells...");
    fillDictionary("old_hint_6_2", "", "", "...Charm Person (PSDF) to force a gesture");
    fillDictionary("old_hint_6_3", "", "", "Charm Monster (PSDD) to take control of a monster, and change its target");
    fillDictionary("old_hint_6_4", "", "", "Conceal movements with Invisibility (PPws) - lower caps means w/s with both hands");
    fillDictionary("old_hint_7_1", "", "", "Hint: Summon stronger monsters in addition to Goblins (SFW)...");
    fillDictionary("old_hint_7_2", "", "", "like Ogre (PSFW), Troll (FPSFW) and Giant (WFPSFW) - notice the pattern ?");
    fillDictionary("old_hint_8_1", "", "", "Hint: The most powerful spell around? That's Finger of Death (PWPFSSSD).");
    fillDictionary("old_hint_8_2", "", "", "...It can't be countered, but it can be mirrored (cw)");
    fillDictionary("old_hint_9_1", "", "", "Hint: Playing with friends can be a lot more fun. Send them an invite.");
    fillDictionary("old_hint_9_2", "", "", "You can also set up a private game to play together.");
    fillDictionary("old_hint_9_3", "", "", "Every time you win against a powerful warlocks your score will go up to reflect your skill.");
    fillDictionary("old_hint_10_1", "", "", "In melee games 3-6 players play at the same time");
    fillDictionary("old_hint_10_2", "", "", "Common spells are Fire and Ice Elementals (cWSSW, cSWWS) and Storms (SWWc, WSSc)");
    fillDictionary("old_hint_10_3", "", "", "Resistance will protect you (WWFP, SSFP)");
    fillDictionary("old_hint_10_4", "", "", "Checkout the full rules at (https://games.ravenblack.net/rules)");
    // https://github.com/Pz1c/WavingHands/issues/297
    fillDictionary("hint_1_1_icon", "", "", "g_s");
    fillDictionary("hint_1_2_icon", "", "", "goblin");
    fillDictionary("hint_1_3_icon", "", "", "magic_missile");
    fillDictionary("hint_1_4_icon", "", "", "RIP2");
    fillDictionary("hint_1_5_icon", "", "", "spellbook");
    fillDictionary("hint_2_1_icon", "", "", "amnesia");
    fillDictionary("hint_2_2_icon", "", "", "target_nobody");
    fillDictionary("hint_3_1_icon", "", "", "goblin");
    fillDictionary("hint_3_2_icon", "", "", "goblin");
    fillDictionary("hint_4_1_icon", "", "", "mshield");
    fillDictionary("hint_4_2_icon", "", "", "heart_small");
    fillDictionary("hint_5_1_icon", "", "", "stars");

    fillDictionary("BotSay_1", "", "", "Hi, warlock, I am a golem created for your training, try to beat me. Every letter represents a hand gesture. A set of gestures like DPP is a spell (Amnesia).");
    fillDictionary("BotSay_2", "", "", "Here are some more useful spells: SD (Magic Missile) deals 1 damage. P (shield) will defend against it. But P/P with both hands will make you surrender.");
    fillDictionary("BotSay_3", "", "", "Enchantments like DPP (Amnesia), DSF (Maladroit), SWD (Fear) disrupt your enemy. When two are cast at the same Warlock they cancel out.");
    fillDictionary("BotSay_4", "", "", "Check your spell book to learn more spells...");
    fillDictionary("BotSay_5", "", "", "Summon a goblin (SFW) to fight for you. Cast it on yourself, and don't forget to set it's target. Stab ('>') is a simple way to hit a goblin. Shield ('P') and Protection (WWP) to defend against it.");
    fillDictionary("BotSay_6", "", "", "Inflict direct damage with Cause light or heavy wounds (WFP, WPFD), and heal up (DFW, DFPW)");
    fillDictionary("BotSay_7", "", "", "There are two ways to counter a spell (WPP, WWS). Cast them at yourself to prevent spells and monsters from hitting you. Cast at an opponent to prevent him from summoning a monster.");
    fillDictionary("BotSay_8", "", "", "You're doing good! Keep up. When you get a bit more training you can start playing against other players and friends.");
    fillDictionary("BotSay_9", "", "", "Games with other players are played in turns. You can play up-to 3 games at the same time for free, and you're given up to 3 days to play each turn.");
    fillDictionary("BotSay_10", "", "", "When you start playing against real players, you'll be seeing a lot of these spells: Charm Person (PSDF) to force a gesture (or a no gesture '-'), and Charm Monster (PSDD) to take control of a monster, and change it's target");
    fillDictionary("BotSay_11", "", "", "You will also see players concealing their movements with Invisibility (PPws - notice the lower caps means you should w and s and both hands together)");
    fillDictionary("BotSay_12", "", "", "Players will summon stronger monsters in addition to Goblins (SFW) - like Ogre (PSFW), Troll (FPSFW) and Giant (WFPSFW) - noticed the pattern ?");
    fillDictionary("BotSay_13", "", "", "The most powerful spell around? That's Finger of Death (PWPFSSSD) to instantly kill an opponent. It can't be countered, but it can be mirrored (cw)");
    fillDictionary("BotSay_14", "", "", "Playing with friends can be a lot more fun. Send them an invite. You can set up a private game to play together.");
    fillDictionary("BotSay_15", "", "", "Every time you win, you can go up the ladder board. When you win against powerful warlocks your elo score will go up too to reflect your skill.");
    fillDictionary("BotSay_16", "", "", "In melee games many 3-6 players play at the same time, often using Fire and Ice Elementals (cWSSW, cSWWS), Storms (SWWc, WSSc) and Protections (WWFP, SSFP)");
    fillDictionary("BotSay_17", "", "", "Want to learn more? checkout the full rules (https://games.ravenblack.net/rules) and join our Facebook community (https://fb.com/WarlocksDuel/)");

    // spell book name
    fillDictionary("cDPW", "Розсіяти магію", "Развеять магию", "Dispel Magic");
    fillDictionary("cSWWS", "Прикликати Крижаного Елементаля", "Призвать Ледяного Елементаля", "Summon Ice Elemental");
    fillDictionary("cWSSW", "Прикликати Полум'яного Елементаля", "Призвать Огненного Елементаля", "Summon Fire Elemental");
    fillDictionary("cw", "Магічне люстерко", "Магическое зеркало", "Magic Mirror");
    fillDictionary("DFFDD", "Блискавка", "Молния", "Lightning Bolt");
    fillDictionary("DFPW", "Лікувати тяжкі рани", "Лечить тяжелые ранения", "Cure Heavy Wounds");
    fillDictionary("DFW", "Лікувати легкі рани", "Лечить легкие ранения", "Cure Light Wounds");
    fillDictionary("DFWFd", "Сліпота", "Слепота", "Blindness");
    fillDictionary("DWFFd", "Сліпота", "Слепота", "Blindness");
    fillDictionary("DPP", "Амнезія", "Амнезия", "Amnesia");
    fillDictionary("DSF", "Незграбність", "Неуклюжесть", "Maladroitness");
    fillDictionary("DSFFFc", "Хвороба", "Болезнь", "Disease");
    fillDictionary("DWSSSP", "Затримка ефекту", "Задержка єфекта", "Delay Effect");    
    fillDictionary("DWWFWD", "Отрута", "Яд", "Poison");
    fillDictionary("FFF", "Параліч", "Паралич", "Paralysis");
    fillDictionary("WFPSFW", "Прикликати Гіганта", "Призвать Гиганта", "Summon Giant");
    fillDictionary("FPSFW", "Прикликати Тролля", "Призвать Тролля", "Summon Troll");
    fillDictionary("PSFW", "Прикликати Огра", "Призвать Огра", "Summon Ogre");
    fillDictionary("SFW", "Прикликати Гобліна", "Призвать Гоблина", "Summon Goblin");
    fillDictionary("FSSDD", "Вогняна куля", "Огненный шар", "Fireball");
    fillDictionary("P", "Щит", "Щит", "Shield");
    fillDictionary("p", "Капітулювати", "Капитулировать", "Surrender");
    fillDictionary("PDWP", "Зняти чари", "Снять колдовство", "Remove Enchantment");
    fillDictionary("PPws", "Невидимість", "Невидимость", "Invisibility");
    fillDictionary("PSDD", "Зачарувати монстра", "Околдовать монстра", "Charm Monster");
    fillDictionary("PSDF", "Зачарувати людину", "Околдовать человека", "Charm Person");
    fillDictionary("PWPFSSSD", "Палець Смерті", "Палец Смерти", "Finger of Death");
    fillDictionary("PWPWWc", "Прискорення", "Ускорение", "Haste");
    fillDictionary("SD", "Магічна стріла", "Магическая стрела", "Magic Missile");
    fillDictionary("SPFP", "Анти чари", "Анти колдовство", "Anti-spell");
    fillDictionary("SPFPSDW", "Постійність", "Постоянство", "Permanency");
    fillDictionary("SPPc", "Зупинка часу", "Остановка времени", "Time Stop");
    fillDictionary("SPPFD", "Зупинка часу", "Остановка времени", "Time Stop");
    fillDictionary("SSFP", "Захист від холоду", "Защита от холода", "Resist Cold");
    fillDictionary("SWD", "Страх", "Страх", "Fear");
    fillDictionary("SWWc", "Пекельний шторм", "Огненный шторм", "Fire Storm");
    fillDictionary("WDDc", "Хлопок Блискавки", "Хлопок Молнии", "Clap of Lightning");
    fillDictionary("WFP", "Нанести легкі рани", "Нанести легкие раны", "Cause Light Wounds");
    fillDictionary("WPFD", "Нанести тяжкі рани", "Нанести тяжелые раны", "Cause Heavy Wounds");
    fillDictionary("WPP", "Магічний захист", "Магическая защита", "Counter Spell");
    fillDictionary("WSSc", "Крижаний шторм", "Ледяной шторм", "Ice Storm");
    fillDictionary("WWFP", "Захист від вогню", "Защита от жара", "Resist Heat");
    fillDictionary("WWP", "Захист", "Захист", "Protection");
    fillDictionary("WWS", "Магічний захист", "Магическая защита", "Counter Spell");
    fillDictionary(">", "Удар", "Удар", "Stab");
    fillDictionary("DSFDFc", "Хвороба", "Болезнь", "Disease");
    fillDictionary("FDF", "Параліч", "Паралич", "Paralysis");
    fillDictionary("FDFD", "Параліч", "Паралич", "Paralysis");

    // spell book short desc
    fillDictionary("p_short_desc", "", "", "Surrender the game ('p' with both hands)");
    fillDictionary("P_short_desc", "", "", "Defend against monsters and stabs");
    fillDictionary("WWP_short_desc", "", "", "Defend against monsters and stabs for 3 turns");
    fillDictionary(">_short_desc", "", "", "Do 1 damage (stab is not a spell)");
    fillDictionary("SD_short_desc", "", "", "Do 1 damage");
    fillDictionary("WFP_short_desc", "", "", "Do 2 damage");
    fillDictionary("WPFD_short_desc", "", "", "Do 3 damage");
    fillDictionary("DFFDD_short_desc", "", "", "Do 5 damage");
    fillDictionary("FSSDD_short_desc", "", "", "Do 5 damage");
    fillDictionary("WDDc_short_desc", "", "", "Do 5 damage, once per battle");
    fillDictionary("DFW_short_desc", "", "", "Heal 1 damage");
    fillDictionary("DFPW_short_desc", "", "", "Heal 2 damage");
    fillDictionary("SFW_short_desc", "", "", "Monster with 1 hp (usually cast at yourself)");
    fillDictionary("PSFW_short_desc", "", "", "Monster with 2 hp (usually cast at yourself)");
    fillDictionary("FPSFW_short_desc", "", "", "Monster with 3 hp (usually cast at yourself)");
    fillDictionary("WFPSFW_short_desc", "", "", "Monster with 4 hp (usually cast at yourself)");
    fillDictionary("DPP_short_desc", "", "", "Opponent repeats last turn gestures");
    fillDictionary("DSF_short_desc", "", "", "Opponent must do same gesture with both hands");
    fillDictionary("SWD_short_desc", "", "", "Opponent may only use 'P', 'W' or '>' gestures");
    fillDictionary("FFF_short_desc", "", "", "Choose opponent's hand. Force it to repeat last gesture. Exceptions: 'F' turns to 'C', 'W' to 'P', 'S' to 'D'");
    fillDictionary("FDF_short_desc", "", "", "Choose opponent's hand. Force it to repeat last gesture. Exceptions: 'F' turns to 'C', 'W' to 'P', 'S' to 'D'");
    fillDictionary("FDFD_short_desc", "", "", "Choose opponent's hand. Force it to repeat last gesture. Exceptions: 'F' turns to 'C', 'W' to 'P', 'S' to 'D'");
    fillDictionary("PSDF_short_desc", "", "", "Choose opponent's hand, then decide his next gesture");
    fillDictionary("PSDD_short_desc", "", "", "Take control of a monster. Cast it at the monster, and choose a new target for that monster to attack");
    fillDictionary("WPP_short_desc", "", "", "Cast it at the target of the spell that you wish to counter (not at the caster). Also acts as a shield");
    fillDictionary("WWS_short_desc", "", "", "Cast it at the target of the spell that you wish to counter (not at the caster). Also acts as a shield");
    fillDictionary("cw_short_desc", "", "", "Reflects a spell back to the caster");
    fillDictionary("cDPW_short_desc", "", "", "Counter all spells, Destroy all monsters and enchantments. Also acts as a shield");
    fillDictionary("PDWP_short_desc", "", "", "Destroy an enchantment (Amensia, Fear, etc.) or a monster");
    fillDictionary("PPws_short_desc", "", "", "You cannot be seen or targeted for 3 turns");
    fillDictionary("DFWFd_short_desc", "", "", "Target cannot see or aim for 3 turns");
    fillDictionary("DWFFd_short_desc", "", "", "Target cannot see or aim for 3 turns");
    fillDictionary("SPFP_short_desc", "", "", "Cut opponent's gestures, making him start a new chain of gestures");
    fillDictionary("SPFPSDW_short_desc", "", "", "Make an enchantment that you cast in the following 3 turns permanent");
    fillDictionary("DWSSSP_short_desc", "", "", "Bank a spell that you cast in the following 3 turns, so you can use it later");
    fillDictionary("DSFFFc_short_desc", "", "", "Opponent will die in 6 turns unless removing the enchantment with DFPW, PDWP, cDPW");
    fillDictionary("DWWFWD_short_desc", "", "", "Opponent will die in 6 turns unless removing the enchantment with PDWP, cDPW");
    fillDictionary("PWPFSSSD_short_desc", "", "", "Opponent dies. This spell cannot be countered, but it can be mirrored");
    fillDictionary("PWPWWc_short_desc", "", "", "Play an extra turn for each of the next 3 turns");
    fillDictionary("SPPc_short_desc", "", "", "Play an extra turn, ignoring enchantments");
    fillDictionary("SPPFD_short_desc", "", "", "Play an extra turn, ignoring enchantments");
    fillDictionary("cWSSW_short_desc", "", "", "Elemental with 3 hp, attacking everyone except those resistant to heat");
    fillDictionary("cSWWS_short_desc", "", "", "Elemental with 3 hp, attacking everyone except those resistant to cold");
    fillDictionary("WWFP_short_desc", "", "", "Enchantment to protect against or to destroy Fire Elemental, and protect against Fire Storm and Fireball");
    fillDictionary("SSFP_short_desc", "", "", "Enchantment to protect against or to destroy Ice Elemental, and protect against Ice Storm");
    fillDictionary("WSSc_short_desc", "", "", "Do 5 damage to all, except those resistant to cold");
    fillDictionary("SWWc_short_desc", "", "", "Do 5 damage to all, except those resistant to heat");


    // spell book desc
    fillDictionary("cDPW_desc",
                   "Це заклинання діє як комбінація Магічного Захисту і Зняття Чар, але його наслідки є універсальними, а не обмежується суб'єктом заклинання. Це закляття зупинить будьяке інше закляття викликане в той же хід (крім іншого Розсіяння Магії), і знімають всі чари з усіх, перш ніж вони будуть мати ефект - крім сліпоти і невидимості які не дадуть побачити жести Чаклуна в той же хід.  Крім того, всі монстри будуть знищені, хоча вони зможуть напасти на цей хід. Удар і Капітуляція вступлять в силу, так як вони не є магічними. Як і Магічний Захист, Розсіювання Магія також виступає в якості Щита для своєї цілі.",
                   "Это заклинание действует как комбинация Магического Защиты и Снятие Чар, но его последствия являются универсальными, а не ограничивается субъектом заклинания. Это заклятие остановит всякое другое заклятие вызваное в тот же ход (помимо другого Рассеяние Магии), и снимают все чары из всех, прежде чем они будут иметь эффект - кроме слепоты и невидимости которые не дадут увидеть жесты Колдуна в тот же ход. Кроме того, все монстры будут уничтожены, хотя они смогут напасть на этот ход. Удар и Капитуляция вступят в силу, так как они не являются магическими. Как и Магический Защита, Рассеивание Магия также выступает в качестве Щита для своей цели.",
                   "This spell acts as a combination of Counter Spell and Remove Enchantment, but its effects are universal rather than limited to the subject of the spell. It will stop any spell cast in the same turn from working (apart from another Dispel Magic spell), and will remove all enchantments from all beings before they have effect - except Blindness and Invisibility still prevent Warlocks from seeing Gestures that turn. In addition, all monsters are destroyed although they can attack that turn. Stab and Surrender still take effect, as they are not magical. As with a Counter Spell, Dispel Magic also acts as a Shield for its subject.");
    fillDictionary("cSWWS_desc",
                   "Це заклинання створює Крижаного елементаля. При застосуванні заклинання елементаль буде створений, навіть якщо ціль закляття не вказана.<br>Елементалі атакують всіх (у тому числі, того хто викликав), пошкоджують будь-якого, хто не захищений від його типу атаки (Жар або Холод), чи Щитом. Елементал наносить 3 очка пошкоджень за хід кожному незахищеному.<br>Елементалі можуть бути вбиті нанесенням 3 або більше очок пошкоджень (Нанести Тяжкі Рани підійде), в цьому випадку елементаль все одно атакує в раунді, в якому він був знищений.<br>Вогняний Елементаль може бути миттєво знищений (не будуть атакувати в цьому ході) Крижаним Штормом, викликом Крижаного Елементаля, Захист від Вогню або Пекельний Шторм. Крім того, кожне з цих заклинань буде нейтралізоване, якщо Вогняний Елементаль присутній на полі, за винятком Захисту від Вогню чи Пекельного Шторму. Вогнений Елементаль нечутливий до Вогнянної кулі.<br>Крижаний Елементаль може бути миттєво знищений (не будуть атакувати в цьому ході) Пекельним Штормом, викликом Елементаля Вогню, Захіст від Холоду, Вогняною кулею чи Крижаним штормом. Кожене з цих заклинань буде нейтралізоване, якщо Крижаний Елементаль присутній на полі, за винятком Захист від холоду або Крижаний шторм.<br>Новий Елементний того ж типу буде заміняти старого.<br>Кастування Вогяної кулі на себе, не захистить від нападу крижаного елементаля (як у випадку з крижаним Штормом). (Кастуйте Вогняну Кулю на елементаля.)<br>Елементалі нападають у тому ж раунді в якому викликані, а також в раунді в якому були фізично знищені.",
                   "Это заклинание создает Ледяного элементаля. При применении заклинания элементаль будет создан, даже если цель заклятие не указана.<br>Элементали атакуют всех (в том числе, того кто вызвал), повреждают любого, кто не защищен от его типа атаки (Жар или Холод), или Щитом. Элементал наносит 3 очка повреждений за ход каждому незащищеному.<br>Элементали могут быть убиты нанесением 3 или более очков повреждений (Нанести Тяжелые Раны подойдет), в этом случае элементаль все равно атакует в раунде, в котором он был уничтожен.<br>Огненный Елементаль может быть мгновенно уничтожен (не будут атаковать в этом ходу) Ледяным Штормом, вызовом Ледяного элементаля, Защита от Огня или Огеннным Штормом. Кроме того, каждое из этих заклинаний будет нейтрализовано, если Огненный Елементаль присутствует на поле, за исключением защиты от Огня или Огенного Шторма. Огненный Елементаль нечувствителен к огненных шарам.<br>Ледяной Елементаль может быть мгновенно уничтожен (не будут атаковать в этом ходу) Огенным Штормом, вызовом элементаля Огня, Защитой от Холода, Огненным шаром или Ледяным штормом. Каждый из этих заклинаний будет нейтрализовано, если Ледяной Елементаль присутствует на поле, за исключением Защита от холода или Ледяной шторм.<br>Новый Элементный того же типа будет заменять старого.<br>Каст Огенного шара на себя, не защитит от нападения ледяного элементаля (как в случае с Ледяным Штормом). (Кастуйте Огненный Шар на элементаля.)<br>Элементали нападают в том же раунде в котором вызваны, а также в раунде в котором физически уничтожены.",
                   "This spell creates an ice elemental. Casting this spell will cause an elemental to be created, even if no target is specified.<br>Elementals attack everyone (including the summoner), damaging anyone who is not resistant to its type (heat or cold), or shielded. They cause three points of damage to each victim, per turn.<br>An elemental can be killed by three or more points of damage, in which case it still attacks for the turn in which it is killed, or may be destroyed by spells of the opposite type.<br>A Fire Elemental is instantly destroyed (and does not attack on the turn) by Ice Storm, Summon Ice Elemental, Resist Fire or Fire Storm. Also, each of these spells will be neutralised if a Fire Elemental is present, except Resist Fire or Fire Storm. A Fire Elemental is unaffected by a Fireball.<br>An Ice Elemental is instantly destroyed (and does not attack on the turn) by Fire Storm, Summon Fire Elemental, Resist Cold, Fireball or Ice Storm. Each of these spells will be neutralised if an Ice Elemental is present, except Resist Cold or Ice Storm.<br>A new Elemental of the same type will supplant the first.<br>Casting a Fireball upon yourself when being attacked by an Ice Elemental is not a defense as it is for an Ice Storm. (Cast Fireball at the elemental.)<br>Elementals first attack on the turn in which they are summoned, and if physically destroyed, also attack on the turn in which they are killed.");
    fillDictionary("cWSSW_desc",
                   "Це заклинання створює Полум'яного елементаля. При застосуванні заклинання елементаль буде створений, навіть якщо ціль закляття не вказана.<br>Елементалі атакують всіх (у тому числі, того хто викликав), пошкоджують будь-якого, хто не захищений від його типу атаки (Жар або Холод), чи Щитом. Елементал наносить 3 очка пошкоджень за хід кожному незахищеному.<br>Елементалі можуть бути вбиті нанесенням 3 або більше очок пошкоджень (Нанести Тяжкі Рани підійде), в цьому випадку елементаль все одно атакує в раунді, в якому він був знищений.<br>Вогняний Елементаль може бути миттєво знищений (не будуть атакувати в цьому ході) Крижаним Штормом, викликом Крижаного Елементаля, Захист від Вогню або Пекельний Шторм. Крім того, кожне з цих заклинань буде нейтралізоване, якщо Вогняний Елементаль присутній на полі, за винятком Захисту від Вогню чи Пекельного Шторму. Вогнений Елементаль нечутливий до Вогнянної кулі.<br>Крижаний Елементаль може бути миттєво знищений (не будуть атакувати в цьому ході) Пекельним Штормом, викликом Елементаля Вогню, Захіст від Холоду, Вогняною кулею чи Крижаним штормом. Кожене з цих заклинань буде нейтралізоване, якщо Крижаний Елементаль присутній на полі, за винятком Захист від холоду або Крижаний шторм.<br>Новий Елементний того ж типу буде заміняти старого.<br>Кастування Вогяної кулі на себе, не захистить від нападу крижаного елементаля (як у випадку з крижаним Штормом). (Кастуйте Вогняну Кулю на елементаля.)<br>Елементалі нападають у тому ж раунді в якому викликані, а також в раунді в якому були фізично знищені.",
                   "Это заклинание создает Огненного элементаля. При применении заклинания элементаль будет создан, даже если цель заклятие не указана.<br>Элементали атакуют всех (в том числе, того кто вызвал), повреждают любого, кто не защищен от его типа атаки (Жар или Холод), или Щитом. Элементал наносит 3 очка повреждений за ход каждому незащищеному.<br>Элементали могут быть убиты нанесением 3 или более очков повреждений (Нанести Тяжелые Раны подойдет), в этом случае элементаль все равно атакует в раунде, в котором он был уничтожен.<br>Огненный Елементаль может быть мгновенно уничтожен (не будут атаковать в этом ходу) Ледяным Штормом, вызовом Ледяного элементаля, Защита от Огня или Огеннным Штормом. Кроме того, каждое из этих заклинаний будет нейтрализовано, если Огненный Елементаль присутствует на поле, за исключением защиты от Огня или Огенного Шторма. Огненный Елементаль нечувствителен к огненных шарам.<br>Ледяной Елементаль может быть мгновенно уничтожен (не будут атаковать в этом ходу) Огенным Штормом, вызовом элементаля Огня, Защитой от Холода, Огненным шаром или Ледяным штормом. Каждый из этих заклинаний будет нейтрализовано, если Ледяной Елементаль присутствует на поле, за исключением Защита от холода или Ледяной шторм.<br>Новый Элементный того же типа будет заменять старого.<br>Каст Огенного шара на себя, не защитит от нападения ледяного элементаля (как в случае с Ледяным Штормом). (Кастуйте Огненный Шар на элементаля.)<br>Элементали нападают в том же раунде в котором вызваны, а также в раунде в котором физически уничтожены.",
                   "This spell creates an fire elemental. Casting this spell will cause an elemental to be created, even if no target is specified.<br>Elementals attack everyone (including the summoner), damaging anyone who is not resistant to its type (heat or cold), or shielded. They cause three points of damage to each victim, per turn.<br>An elemental can be killed by three or more points of damage, in which case it still attacks for the turn in which it is killed, or may be destroyed by spells of the opposite type.<br>A Fire Elemental is instantly destroyed (and does not attack on the turn) by Ice Storm, Summon Ice Elemental, Resist Fire or Fire Storm. Also, each of these spells will be neutralised if a Fire Elemental is present, except Resist Fire or Fire Storm. A Fire Elemental is unaffected by a Fireball.<br>An Ice Elemental is instantly destroyed (and does not attack on the turn) by Fire Storm, Summon Fire Elemental, Resist Cold, Fireball or Ice Storm. Each of these spells will be neutralised if an Ice Elemental is present, except Resist Cold or Ice Storm.<br>A new Elemental of the same type will supplant the first.<br>Casting a Fireball upon yourself when being attacked by an Ice Elemental is not a defense as it is for an Ice Storm. (Cast Fireball at the elemental.)<br>Elementals first attack on the turn in which they are summoned, and if physically destroyed, also attack on the turn in which they are killed.");
    fillDictionary("cw_desc",
                   "Будь яке закляття направлене на об'єкт, захищений Чарівним Люстерком відбивається назад на заклинателя. Чарівне люстерко захищає тільки під час раунду в якому він було скастоване. Захист працює від заклять таки як Магічна Стріла та Блискавка, але не захищае від атак монстрів та ударів Чаклунів. Чарівне люстерко не буде відхилятися заклинання Чарівника, якого захищае. Для чар (наприклад Зачаклувати людину), якщо закляття відбивається від чарівного люстерка, то ціль закляття стає заклинателем, і отримує можливість змінити жести оригінального заклинателя. Магічний захист повністю поглинає Магічне Люстерко якщо накладене в той же хід. Люстерко не має жодного впливу на закляття, які діють на більш ніж одну людину (наприклад, Пекельний Шторм). Закляття може відбиваеться тільки 1 раз, тобто якщо чаклун А захистився Люстерком і кинув на чаклуна Б (який також захистився люстерком) блискавку, то чаклун А втратить 5 очок життя. Неочікувано але Чарівне Люстерко може відбити Палець Смерті.",
                   "Любое заклинание направлено на объект, защищенный Волшебным Зеркалом отражается обратно на заклинателя. Волшебное зеркальце защищает только во время раунда в котором он было скастовано. Защита работает от заклинаний таких как Магическая Стрела и Молния, но не защищает от атак монстров и ударов Колдунов. Волшебное зеркальце не будет отклонять заклинания Волшебника, которого защищает. Для чар (например Заколдовать Человека)​​, если заклятие отражается от волшебного зеркальца, то цель заклинания становится заклинателем, и получает возможность изменить жесты оригинального заклинателя. Магическая защита полностью поглощает Магическое Зеркало наложенное в тот же ход. Зеркальце не имеет никакого влияния на заклинания, которые действуют на более чем одного человека (например, Огненый Шторм). Заклятие может отражаться только 1 раз, т.е. если колдун А защитился зеркалом и бросил на колдуна Б (также защитился зеркалом) молнию, то колдун А потеряет 5 очков жизни. Неожиданно, но Волшебное Зеркало может отразить Палец Смерти.",
                   "Any spell cast at a subject protected by Magic Mirror is reflected back upon the caster of the spell. The Magic Mirror protects only during the turn in which it was cast. The protection includes spells like Magic Missile and Lightning Bolt but does not include attacks by monsters or stabs from Warlocks. A Magic Mirror will not deflect spells cast by the Warlock that the mirror protects. For the purpose of enchantments (eg. Charm Person), if a spell is reflected from a Magic Mirror, the original target becomes the caster, and gets to modify the gestures of the original caster. The mirror is countered totally if a Counter Spell is cast on the subject in the same turn as the mirror. The mirror has no effect on spells which affect more than one person (such as Fire Storm). In the unlikely event of a spell being cast such that it would reflect from more than one Magic Mirror (this requires the collusion of three Warlocks, or some help from a Delay Effect), the spell is reflected only once. See also the unexpected.<br>Suprise that Finger of Deth can be reflected by a Magic Mirror. The surprise catch to this is that a Magic Mirror can be countered by a Counter Spell. So, if Bob casts Finger of Death and Counter Spell at Tim, and Tim casts Magic Mirror at himself, the Magic Mirror will be countered and the Finger of Death will still succeed; Tim dies.");
    fillDictionary("DFFDD_desc",
                   "Ціль цього заклинання простраждає від удару блискавки і отримає п'ять очок ушкоджень. Щит не захищае від цього закляття, але Магчний Захист допоможе, Магічне Люстерко ;) навіть краще.",
                   "Цель этого заклинания простраждае от удара молнии и получит пять очков повреждений. Щит не защищает от этого заклинания, но Магчний Защита поможет, Магическое Зеркальце ;) даже лучше.",
                   "The subject of this spell is hit by a bolt of lightning and sustains five points of damage. A Shield spell offers no defence.");
    fillDictionary("DFPW_desc",
                   "Це заклинання подібно закляттю Лікування Легких Ран, але додає два очка здоров'я. Також це заклинання виліковує Хворобу.",
                   "Это заклинание подобно заклятию Лечение Легких Ран, но добавляет два очка здоровья. Также это заклинание излечивает Болезнь.",
                   "This spell is similar to Cure Light Wounds in effect but two points of damage are cured. This spell will also cure any diseases the subject might have at the time.");
    fillDictionary("DFW_desc",
                   "Ціль закляття лікується на одне очко здоров'я, до максимуму його вихідного здоров'я плюс 1. Якщо шкода заподіяна в той же хід і в іншому випадку привести до смерті суб'єкта (тобто Якщо чаклун має 5 очок здоров'я, і його вдарили по блискавка), то ціль закляття виживає.",
                   "Цель заклинания лечится на одно очко здоровья, до максимума его исходного здоровья плюс 1. Если ущерб причинен в тот же ход и в другом случае привести к смерти субъекта (т.е. Если колдун имеет 5 очков здоровья, и его ударили по молния), то цель заклинания выживает.",
                   "The subject is cured by one Health point, up to a maximum of their starting health plus 1. If damage is inflicted in the same turn which would otherwise result in the subject's death (ie. if they are on 5 Health, and hit by a Lightning Bolt), the subject survives.");
    fillDictionary("DFWFd_desc",
                   "Протягом наступних 3 ходів не рахуючи того, в якій було закляття, об'єкт не в змозі бачити. Якщо він Чернокнижник, він не може бачити, що жести своїх опонентів, хоча він буде поінформований про наслідки заклинань. Він може контролювати своїх монстри, але він не може направити свої заклинання на цілі, які він не може бачити. Засліплені монстри (включаючи Елементалів) миттєво знищується і не можуть атакувати в цьому ходу.",
                   "В течение следующих 3 ходов кроме того, в котором было заклятие, объект не в состоянии видеть. Если он Чернокнижник, он не может видеть, что жесты своих оппонентов, хотя он будет проинформирован о последствиях заклинаний. Он может контролировать своих монстры, но он не может направить свои заклинания на цели, которые он не может видеть. Ослепленные монстры (включая элементалей) мгновенно уничтожается и не могут атаковать в этом ходу.",
                   "For the next 3 turns not including the one in which the spell was cast, the subject is unable to see. If he is a Warlock, he cannot tell what his opponents' gestures are, although he will be informed of the effects of spells cast. He can control his monsters (he can say 'Attack that cursed Zarquon'), but he cannot direct his spells at things that he cannot see. Blinded monsters (including Elementals) are instantly destroyed and cannot attack in that turn.");
    fillDictionary("DPP_desc",
                   "Якщо суб'єкт цього заклинання Чернокнижник, наступний раунд він буде повторить тіж жести які зробив у поточному раунді.<br>Якщо предметом заклинання це чудовисько або елементал, він забуває атакувати в цьому раунді.<br>Якщо об'єкт є також ціллю Паралічу, Заплутаності, Зачарування Людини, Зачарування Монстра, Страху або іншої Амнезії то жодне з цих заклять не спрацює.",
                   "Если субъект этого заклинания Чернокнижник, следующий раунд он будет повторить те же жесты сделанных в текущем раунде.<br>Если предметом заклинание это чудовище или Элементал, он забывает атаковать в этом раунде.<br>Если объект является также целью Паралича, Запутаности, Очарование Человека, Очарование Монстра, Страха или иной Амнезии то ни одно из этих заклинаний не сработает.",
                   "If the subject of this spell is a Warlock, next turn he will repeat identically the gestures he made in the current turn.<br>If the subject of the spell is a monster or elemental, it forgets to attack in that round.<br>If the subject is simultaneously the subject of any of Paralysis, Confusion, Charm Person, Charm Monster, Fear or another Amnesia then none of the spells work.");
    fillDictionary("old_DSF_desc",
                   "Якщо суб'єкт цього закляття Чорнокнижник, наступний раунд він робить жести як звичайно, як зазвичай. Але сервер випадковим чином визначае яка рука була вражена закляттям і замінить жест на випадковий.<br>Якщо суб'єкт цього закляття чудовисько, він атакує випадкову жертву.<br>Якщо об'єкт є також ціллю Паралічу, Заплутаності, Зачарування Людини, Зачарування Монстра, Страху або Амнезії то жодне з цих заклять не спрацює.<br>В Maladroit грі, закляття Плутанина замінюється закляттям Незграбність (жести: DSF): Якщо суб'єкт цього закляття Чорнокнижник, в наступному раунді він повинен вибрати тільки один жест, і виконаю його обома руками (тобто. якщо він вибирає P, він здасться).<br>Якщо предметом закляття це чудовисько, його атака не відбувається в цей раунд. Заклинання не впливає елементалів. Як і з закляттям Плутанина, воно скасовується тими чарами.",
                   "Если субъект этого заклинания Чернокнижник, следующий раунд он делает жесты как обычно, как обычно. Но сервер случайным образом определяет какая рука была поражена заклятием и заменит жест на случайный.<br>Если субъект этого заклинания чудовище, он атакует случайную жертву.<br>Если объект является также целью Паралича, Запутаности, Очарование Человека, Очарование Монстра, Страха или иной Амнезии то ни одно из этих заклинаний не сработает.<br>В Maladroit игре, заклятие Путаница заменяется заклинанием Неуклюжесть (жесты: DSF): Если субъект этого заклинания Чернокнижник, в следующем раунде он должен выбрать только один жест, и исполню его обеими руками (то есть. Если он выбирает P, он капитулирует).<br>Если предметом заклятие это чудовище, его атака не происходит в этот раунд. Заклинание не влияет элементалей. Как и с заклятием Путаница, оно отменяется теми чарами.",
                   "If the subject of this spell is a Warlock, next turn he submits orders as usual. However, during resolution of the round, the server will randomly determine which hand is affected, and will randomly replace the ordered gesture with one of the eight possible gestures.<br>If the subject of the spell is a monster, it attacks a random victim that turn.<br>If the subject is also the subject of any of: Paralysis, Amnesia, another Confusion, Charm Person, Charm Monster, or Fear, none of the spells work.<br>In a Maladroit game, the Confusion spell is replaced with Maladroitness (gestures: DSF): If the subject of this spell is a Warlock, on the next round he must select only a single gesture, and will perform it with both hands (ie. if he selects P, he will surrender).<br>If the subject of the spell is a monster, its attack fails this round. The spell does not affect elementals. As with Confusion, it cancels with the same other enchantments.");
     fillDictionary("DSF_desc",
                   "Якщо суб'єкт цього закляття Чорнокнижник, наступний раунд він робить жести як звичайно, як зазвичай. Але сервер випадковим чином визначае яка рука була вражена закляттям і замінить жест на випадковий.<br>Якщо суб'єкт цього закляття чудовисько, він атакує випадкову жертву.<br>Якщо об'єкт є також ціллю Паралічу, Заплутаності, Зачарування Людини, Зачарування Монстра, Страху або Амнезії то жодне з цих заклять не спрацює.<br>В Maladroit грі, закляття Плутанина замінюється закляттям Незграбність (жести: DSF): Якщо суб'єкт цього закляття Чорнокнижник, в наступному раунді він повинен вибрати тільки один жест, і виконаю його обома руками (тобто. якщо він вибирає P, він здасться).<br>Якщо предметом закляття це чудовисько, його атака не відбувається в цей раунд. Заклинання не впливає елементалів. Як і з закляттям Плутанина, воно скасовується тими чарами.",
                   "Если субъект этого заклинания Чернокнижник, следующий раунд он делает жесты как обычно, как обычно. Но сервер случайным образом определяет какая рука была поражена заклятием и заменит жест на случайный.<br>Если субъект этого заклинания чудовище, он атакует случайную жертву.<br>Если объект является также целью Паралича, Запутаности, Очарование Человека, Очарование Монстра, Страха или иной Амнезии то ни одно из этих заклинаний не сработает.<br>В Maladroit игре, заклятие Путаница заменяется заклинанием Неуклюжесть (жесты: DSF): Если субъект этого заклинания Чернокнижник, в следующем раунде он должен выбрать только один жест, и исполню его обеими руками (то есть. Если он выбирает P, он капитулирует).<br>Если предметом заклятие это чудовище, его атака не происходит в этот раунд. Заклинание не влияет элементалей. Как и с заклятием Путаница, оно отменяется теми чарами.",
                   "If the subject is also the subject of any of: Paralysis, Amnesia, another Maladroit, Charm Person, Charm Monster, or Fear, none of the spells work. If the subject of this spell is a Warlock, on the next round he must select only a single gesture, and will perform it with both hands (ie. if he selects P, he will surrender).<br>If the subject of the spell is a monster, its attack fails this round. The spell does not affect elementals.");
    fillDictionary("DSFFFc_desc",
                   "Ціль цього закляття негайно захворіває смертельною (не заразною) хворобою, яке вб'є його наприкінці шостого раунду після захворювання. Лічильник в рядку чорнокнижника показуватиме кількість ходів до смерті. Коли лічильник досягне нуля, чорнокнижник вмирає. Хвороба лікується Видаленням Чар або Вилікувати Важкі Рани або Розсіювання Магії. Якщо предметом заклинання це чудовисько, він відразу ж помирає, але все ще може атакувати в цьому ході.",
                   "Цель этого заклинания немедленно заболевает смертельной (не заразно) болезнью, которое убьет его в конце шестого раунда после заболевания. Счетчик в строке чернокнижника показывать количество ходов до смерти. Когда счетчик достигнет нуля, чернокнижник умирает. Болезнь лечится Удалением Чар или Вылечить Тяжелые Раны или Рассеивание Магии. Если предметом заклинание это чудовище, он сразу же умирает, но все еще может атаковать в этом ходу.",
                   "The subject of this spell immediately contracts a deadly (non contagious) disease which will kill him at the end of the sixth turn following the one upon which the spell is cast. A counter in the warlock's status bar indicates the number of turns he has left to live. When the counter reaches zero, the warlock dies. The malady is cured by Remove Enchantment or Cure Heavy Wounds or Dispel Magic in the meantime. If the subject of the spell is a monster, it dies immediately, but still gets to attack that turn.");
    fillDictionary("DSFDFc_desc",
                   "Ціль цього закляття негайно захворіває смертельною (не заразною) хворобою, яке вб'є його наприкінці шостого раунду після захворювання. Лічильник в рядку чорнокнижника показуватиме кількість ходів до смерті. Коли лічильник досягне нуля, чорнокнижник вмирає. Хвороба лікується Видаленням Чар або Вилікувати Важкі Рани або Розсіювання Магії. Якщо предметом заклинання це чудовисько, він відразу ж помирає, але все ще може атакувати в цьому ході.",
                   "Цель этого заклинания немедленно заболевает смертельной (не заразно) болезнью, которое убьет его в конце шестого раунда после заболевания. Счетчик в строке чернокнижника показывать количество ходов до смерти. Когда счетчик достигнет нуля, чернокнижник умирает. Болезнь лечится Удалением Чар или Вылечить Тяжелые Раны или Рассеивание Магии. Если предметом заклинание это чудовище, он сразу же умирает, но все еще может атаковать в этом ходу.",
                   "The subject of this spell immediately contracts a deadly (non contagious) disease which will kill him at the end of the sixth turn following the one upon which the spell is cast. A counter in the warlock's status bar indicates the number of turns he has left to live. When the counter reaches zero, the warlock dies. The malady is cured by Remove Enchantment or Cure Heavy Wounds or Dispel Magic in the meantime. If the subject of the spell is a monster, it dies immediately, but still gets to attack that turn.");
    fillDictionary("DWFWd_desc",
                   "Протягом наступних 3 ходів не рахуючи того, в якій було закляття, об'єкт не в змозі бачити. Якщо він Чернокнижник, він не може бачити, що жести своїх опонентів, хоча він буде поінформований про наслідки заклинань. Він може контролювати своїх монстри, але він не може направити свої заклинання на цілі, які він не може бачити. Засліплені монстри (включаючи Елементалів) миттєво знищується і не можуть атакувати в цьому ходу.",
                   "В течение следующих 3 ходов кроме того, в котором было заклятие, объект не в состоянии видеть. Если он Чернокнижник, он не может видеть, что жесты своих оппонентов, хотя он будет проинформирован о последствиях заклинаний. Он может контролировать своих монстры, но он не может направить свои заклинания на цели, которые он не может видеть. Ослепленные монстры (включая элементалей) мгновенно уничтожается и не могут атаковать в этом ходу.",
                   "For the next 3 turns not including the one in which the spell was cast, the subject is unable to see. If he is a Warlock, he cannot tell what his opponents' gestures are, although he will be informed of the effects of spells cast. He can control his monsters (he can say 'Attack that cursed Zarquon'), but he cannot direct his spells at things that he cannot see. Blinded monsters (including Elementals) are instantly destroyed and cannot attack in that turn.");
    fillDictionary("DWFFd_desc",
                   "Протягом наступних 3 ходів не рахуючи того, в якій було закляття, об'єкт не в змозі бачити. Якщо він Чернокнижник, він не може бачити, що жести своїх опонентів, хоча він буде поінформований про наслідки заклинань. Він може контролювати своїх монстри, але він не може направити свої заклинання на цілі, які він не може бачити. Засліплені монстри (включаючи Елементалів) миттєво знищується і не можуть атакувати в цьому ходу.",
                   "В течение следующих 3 ходов кроме того, в котором было заклятие, объект не в состоянии видеть. Если он Чернокнижник, он не может видеть, что жесты своих оппонентов, хотя он будет проинформирован о последствиях заклинаний. Он может контролировать своих монстры, но он не может направить свои заклинания на цели, которые он не может видеть. Ослепленные монстры (включая элементалей) мгновенно уничтожается и не могут атаковать в этом ходу.",
                   "For the next 3 turns not including the one in which the spell was cast, the subject is unable to see. If he is a Warlock, he cannot tell what his opponents' gestures are, although he will be informed of the effects of spells cast. He can control his monsters (he can say 'Attack that cursed Zarquon'), but he cannot direct his spells at things that he cannot see. Blinded monsters (including Elementals) are instantly destroyed and cannot attack in that turn.");
    fillDictionary("DWSSSP_desc",
                   "Це заклинання працює тільки тоді, коли накладається на Чорнокнижник. Один заклинань яке він завершує, за умови, що на одному з найближчих 3 ходів (не включаючи той хід, в якому скастоване), може бути збережено для використання на майбутне. Після збереження Чаклун може 1 раз використатти закляття з тимимж параметрами і цілюю як в момент збереження. Збереженне закляття може ьути стерте Розсіянням Магії та Зняттям Чар",
                   "Это заклинание работает только тогда, когда накладывается на Чернокнижник. Один заклинаний которое он завершает, при условии, что на одном из ближайших 3 ходов (не включая тот ход, в котором скастоване), может быть сохранено для использования на будущее. После сохранения Колдун может 1 раз використатты заклятие с тимимж параметрами и целю как в момент сохранения. Сохранение заклятие может ьуты стерто рассеянии Магии и Снятием Чар",
                   "This spell only works if cast upon a Warlock. One of the next spells he completes, provided it is on one of the next 3 turns (not including the turn in which Delay Effect is cast), is \"banked\" until needed, i.e. it fails to work until its caster desires. The spell to be banked can be selected with a radio button while Delay Effect is active. Note that spells banked are those cast by the subject not those cast at him. The target of the spell is banked along with the spell itself (i.e. the complete effect of the spell, including target, is what gets saved). Remember that Surrender is not a spell. A Warlock may only have one spell banked at any one time. Banking a second spell will overwrite the first. When a spell is banked, a Warlock can fire it by checking the 'Fire Banked Spell' option which will appear. A banked spell will not be removed by Dispel Magic or Remove Enchantment. To counter a banked spell, you must cast the counter-spell on the turn the spell is fired - you cannot counter it on the turn in which it is banked.");
    fillDictionary("DWWFWD_desc",
                   "Це те ж саме як Хвороба, але Вилікувати Важкі Рани не допомагає.",
                   "Это то же самое как Болезнь, но Вылечить Тяжелые Раны не помогает.",
                   "This is the same as the disease spell except that Cure Heavy Wounds does not stop its effects.");
    fillDictionary("FFF_desc",
                   "Якщо предметом заклинання Чернокнижник, то у наступному раунді заклинатель вибирає один з рук зачаклованого і в цьому раунді, вибрана рука паралізована. Якщо Чорнокнижник вже має паралізовану руку, того ж заклинателя, то ця ж рука буде паралізована знову.<br>Рука будучи паралізованою замінює жести C, S або W на F, D або P відповідно, для наступного раунду. В іншому випадку вона буде повторювати жест попереднього раунду (удар також).<br>Оновлення: у грі \"ParaFC\" паралізована рука перетворює F у C і навпаки C у F.<br>Якщо предметом заклинання це чудовисько або елементал воно просто не атакує в раунді, в якому було паралізоване.<br>Якщо об'єкт є також ціллю Паралічу, Заплутаності, Зачарування Людини, Зачарування Монстра, Страху або іншої Амнезії то жодне з цих заклять не спрацює.",
                   "Если предметом заклинание Чернокнижник, то в следующем раунде заклинатель выбирает один из рук околдованого и в этом раунде, выбранная рука парализована. Если Чернокнижник уже имеет парализованную руку, того же заклинателя, то эта же рука будет парализована снова.<br>Рука будучи парализованной заменяет жесты C, S или W на F, D или P соответственно, для следующего раунда. В противном случае она будет повторять жест предыдущего раунда (удар тоже).<br>Обновление: в игре \"ParaFC\" парализована рука превращает F в C и наоборот C в F.<br>Если предметом заклинание это чудовище или элементаль воно просто не атакует в раунде, в котором было парализовано.<br>Если объект является также целью Паралича, Запутаности, Очарование Человека, Очарование Монстра, Страха или иной Амнезии то ни одно из этих заклинаний не сработает.",
                   "If the subject of the spell is a Warlock, then for the following turn the caster selects one of the Warlock's hands and on that turn, the selected hand is paralysed. If the target Warlock already had a paralysed hand, paralysed by the same caster, the caster will not have the choice - the same hand will be paralysed again.<br>If the hand being paralysed is, at the time the spell is cast, performing a F, S or W it is instead paralysed into C, D or P respectively, for the next turn. Otherwise it will repeat the original position (this allows repeated stabs).<br>If the subject of the spell is a monster or an elemental it simply does not attack in the turn in which the spell was cast.<br>If the subject of the spell is also the subject of any of Amnesia, Confusion, Charm Person, Charm Monster, Fear or another Paralysis, none of the spells work.");
    fillDictionary("FDF_desc",
                   "Якщо предметом заклинання Чернокнижник, то у наступному раунді заклинатель вибирає один з рук зачаклованого і в цьому раунді, вибрана рука паралізована. Якщо Чорнокнижник вже має паралізовану руку, того ж заклинателя, то ця ж рука буде паралізована знову.<br>Рука будучи паралізованою замінює жести C, S або W на F, D або P відповідно, для наступного раунду. В іншому випадку вона буде повторювати жест попереднього раунду (удар також).<br>Оновлення: у грі \"ParaFC\" паралізована рука перетворює F у C і навпаки C у F.<br>Якщо предметом заклинання це чудовисько або елементал воно просто не атакує в раунді, в якому було паралізоване.<br>Якщо об'єкт є також ціллю Паралічу, Заплутаності, Зачарування Людини, Зачарування Монстра, Страху або іншої Амнезії то жодне з цих заклять не спрацює.",
                   "Если предметом заклинание Чернокнижник, то в следующем раунде заклинатель выбирает один из рук околдованого и в этом раунде, выбранная рука парализована. Если Чернокнижник уже имеет парализованную руку, того же заклинателя, то эта же рука будет парализована снова.<br>Рука будучи парализованной заменяет жесты C, S или W на F, D или P соответственно, для следующего раунда. В противном случае она будет повторять жест предыдущего раунда (удар тоже).<br>Обновление: в игре \"ParaFC\" парализована рука превращает F в C и наоборот C в F.<br>Если предметом заклинание это чудовище или элементаль воно просто не атакует в раунде, в котором было парализовано.<br>Если объект является также целью Паралича, Запутаности, Очарование Человека, Очарование Монстра, Страха или иной Амнезии то ни одно из этих заклинаний не сработает.",
                   "If the subject of the spell is a Warlock, then for the following turn the caster selects one of the Warlock's hands and on that turn, the selected hand is paralysed. If the target Warlock already had a paralysed hand, paralysed by the same caster, the caster will not have the choice - the same hand will be paralysed again.<br>If the hand being paralysed is, at the time the spell is cast, performing a C, S or W it is instead paralysed into F, D or P respectively, for the next turn. Otherwise it will repeat the original position (this allows repeated stabs).<br>UPDATE: A \"ParaFC\" game paralyses F to C instead of C to F. See Challenges for how to start a ParaFC game.<br>If the subject of the spell is a monster or an elemental it simply does not attack in the turn in which the spell was cast.<br>If the subject of the spell is also the subject of any of Amnesia, Confusion, Charm Person, Charm Monster, Fear or another Paralysis, none of the spells work.");
    fillDictionary("FDFD_desc",
                   "Якщо предметом заклинання Чернокнижник, то у наступному раунді заклинатель вибирає один з рук зачаклованого і в цьому раунді, вибрана рука паралізована. Якщо Чорнокнижник вже має паралізовану руку, того ж заклинателя, то ця ж рука буде паралізована знову.<br>Рука будучи паралізованою замінює жести C, S або W на F, D або P відповідно, для наступного раунду. В іншому випадку вона буде повторювати жест попереднього раунду (удар також).<br>Оновлення: у грі \"ParaFC\" паралізована рука перетворює F у C і навпаки C у F.<br>Якщо предметом заклинання це чудовисько або елементал воно просто не атакує в раунді, в якому було паралізоване.<br>Якщо об'єкт є також ціллю Паралічу, Заплутаності, Зачарування Людини, Зачарування Монстра, Страху або іншої Амнезії то жодне з цих заклять не спрацює.",
                   "Если предметом заклинание Чернокнижник, то в следующем раунде заклинатель выбирает один из рук околдованого и в этом раунде, выбранная рука парализована. Если Чернокнижник уже имеет парализованную руку, того же заклинателя, то эта же рука будет парализована снова.<br>Рука будучи парализованной заменяет жесты C, S или W на F, D или P соответственно, для следующего раунда. В противном случае она будет повторять жест предыдущего раунда (удар тоже).<br>Обновление: в игре \"ParaFC\" парализована рука превращает F в C и наоборот C в F.<br>Если предметом заклинание это чудовище или элементаль воно просто не атакует в раунде, в котором было парализовано.<br>Если объект является также целью Паралича, Запутаности, Очарование Человека, Очарование Монстра, Страха или иной Амнезии то ни одно из этих заклинаний не сработает.",
                   "If the subject of the spell is a Warlock, then for the following turn the caster selects one of the Warlock's hands and on that turn, the selected hand is paralysed. If the target Warlock already had a paralysed hand, paralysed by the same caster, the caster will not have the choice - the same hand will be paralysed again.<br>If the hand being paralysed is, at the time the spell is cast, performing a C, S or W it is instead paralysed into F, D or P respectively, for the next turn. Otherwise it will repeat the original position (this allows repeated stabs).<br>UPDATE: A \"ParaFC\" game paralyses F to C instead of C to F. See Challenges for how to start a ParaFC game.<br>If the subject of the spell is a monster or an elemental it simply does not attack in the turn in which the spell was cast.<br>If the subject of the spell is also the subject of any of Amnesia, Confusion, Charm Person, Charm Monster, Fear or another Paralysis, none of the spells work.");
    fillDictionary("WFPSFW_desc",
                   "Це заклинання такий же, як Прикликання Гобліна, але Гігант завдає і руйнується 4 одиницями пошкодження, а не 1.",
                   "Это заклинание такой же, как Вызов Гоблина, но Гигант наносит и разрушается 4 единицами повреждения, а не 1.",
                   "This spell is the same as Summon Goblin but the giant created inflicts and is destroyed by four points of damage rather than one.");
    fillDictionary("FPSFW_desc",
                   "Це заклинання такий же, як Прикликання Гобліна, але Тролль завдає і руйнується 3 одиницями пошкодження, а не 1.",
                   "Это заклинание такой же, как Вызов Гоблина, но Тролль наносит и разрушается 3 единицами повреждения, а не 1.",
                   "This spell is the same as Summon Goblin but the troll created inflicts and is destroyed by three points of damage rather than one.");
    fillDictionary("PSFW_desc",
                   "Це заклинання такий же, як Прикликання Гобліна, але Огр завдає і руйнується 2 одиницями пошкодження, а не 1.",
                   "Это заклинание такой же, как Вызов Гоблина, но Огр наносит и разрушается 2 единицами повреждения, а не 1.",
                   "This spell is the same as Summon Goblin but the ogre created inflicts and is destroyed by two points of damage rather than one.");
    fillDictionary("SFW_desc",
                   "Це заклинання викликає гобліна підконтрольного цілі закляття. Увага, ціль закляття стає власником а не жертвою монстра! Гоблін може атакувати в той же хід що він був викликаний. Своєю атакою він забирає одне очко здоров'я своєї жертви. Гобліна можна знищити якщо нанести йому 1 очко шкоди (Удар, Магічна Стріла підійде). Ваш монстр може бути вкрадений за допомогою Зачарування Монстра і це може статись в той же хід що і виклик монстра. Також майте на увазі, якщо ціль закляття захищена Магічним Захистом монстр не буде викликаний. Також якщо цілью закляття є монстр, то власником нового монстру стане власник монстра на якого направлене закляття.",
                   "Это заклинание вызывает гоблина подконтрольного цели заклинания. Внимание цель заклинания становится владельцем а не жертвой монстра! Гоблин может атаковать в тот же ход что он был вызван. Своей атакой он забирает одно очко здоровья своей жертвы. Гоблина можно уничтожить если нанести ему 1 очко вреда (Удар, Магическая Стрела подойдет). Ваш монстр может быть украден с помощью Очарование Монстра и это может произойти в тот же ход что и вызов монстра. Также имейте в виду, если цель заклинания защищена Магическим Защитой монстр не будет вызван. Также если цель заклинания есть монстр, то владельцем нового монстра станет владелец монстра на которого направлено заклинание.",
                   "This spell creates a goblin under the control of the target of the spell (or the target's controller, if the target is a monster). Note that the spell target becomes the controller, not the victim! The goblin can attack immediately and its victim will be an opponent of its controller. It does one point of damage to its victim per turn and is destroyed after one point of damage is inflicted upon it. The summoning spell cannot be cast at an elemental, and if cast at something which doesn't exist, the spell has no effect. Be aware, target of spell become monster's owner not a victim. Your monster can be stolen with Charm Monster and it's possible to steal monster in the same turm that summon. Also take in mind, if target of spell covered with Counter Spell monster not summoned.");
    fillDictionary("FSSDD_desc",
                   "Ціль закляття буде вдарена Вогненою Кулею та отримає п'ять одиниць пошкодження, якщо він не стійкий до вогню. Якщо Вогняна куля викликана одночасно із Крижаним штормом, то ціль Вогняної Кулі не постраждає ні від Кулі ні від шторму, хоча на інших Шторм вплине як звичайно. Якщо Вогняна Куля спрямована на Крижаного Елементаля, то елементаль буде знищений перш, ніж він зможе атакувати.",
                   "Цель заклятие получит удар Огеннім Шаром и получит пять очек урона, если он не устойчива к огню. Если Огненный шар вызван одновременно с Ледяным штормом, то цель Огненного Шара не пострадает ни от Шара ни от Шторма, хотя на других Шторм повлияет как обычно. Если Огненный Шар направлен ​​на Ледяного элементаля, то элементаль будет уничтожен прежде, чем он сможет атаковать.",
                   "The subject of this spell is hit by a ball of fire and sustains five points of damage unless he is resistant to fire. If at the same time an Ice Storm prevails, the subject of the Fireball is instead not harmed by either spell, although the storm will affect others as normal. If directed at an ice elemental, the fireball will destroy it before it can attack.");
    fillDictionary("P_desc",
                   "Це закляття захистить об'єкт від всіх атак прикликаних монстрів, закляття Магічної стрілиб а також від ударів Чаклунів. Щит блокує будь яку кількість атак, але діє тільки протягом 1 раунду. Щит захищає об'єкт протягом раунду в якому скастований",
                   "Это заклятие защитит обект от всех атак призваных существ, заклятия Магическая стрела, и ударов Колдунов. Щит блокирует любое количество атак, но действует только 1 раунд. Щит защищает обект в тот раунд в котором он скастован",
                   "This spell protects the subject from all attacks from summoned monsters, from Magic Missile spells, and from stabs by Warlocks. The shield will block any number of such attacks but lasts for only one round. The shield protects the subject on the turn in which it is cast.");
    fillDictionary("p_desc",
                   "Чаклун який капітулював припиняє дуєль і визнає поразку",
                   "Колдун который капитулировал покидает дуель и признает поражение",
                   "This is not a spell and consequently cannot be cast at anyone. The Warlock making these gestures, irrespective of whether they terminate spells or not, surrenders and the contest is over. If the gestures do complete spells, those spells will be cast as usual - the surrender does not happen until the end of the round. The surrendering Warlock is deemed to have lost unless his gestures completed spells which killed all remaining opponents, in which case he is deemed to have won. Two or more simultaneous surrenders count as a draw, in the same way as two or more simultaneous deaths - none of the Warlocks gain or lose any points, even those which died earlier in the Battle. It is a skill for Warlocks to work their spells so that they never accidentally perform 2 P gestures simultaneously. It is too late to surrender on the turn you are killed - if you are killed as you make the surrender gesture, you will still die. However, if you live to the end of the turn, the referee will cure any diseases, poisons etc. in time for your next battle.");
    fillDictionary("PDWP_desc",
                   "Припиняє дію всіх Зачаровуючих заклять що були накладені на об'єкт включаючи ті що були накладені в тому ж раунді що і Зняти чари.<br>Ефекти що вже відбулися не відміняються, для прикладу жертва закляття Сліпота не побачить жести противника які були виконані, навіть жести з раунда коли зняття чар було скастоване і Сліпота пропала. Всі ефекти будуть відмінені без вибору, також Зняття Чар знищуе викликаного монстра якщо було скастоване на нього, але монстр ще може атакувати цього раунду. Чаклуни немають ніяких негативних ефектів від закляття окрім знаття чар.",
                   "Прекращает действие всех завораживающий заклинаний которые были наложены на объект включая те, что были наложены в том же раунде что и Снять чары.<br>Эффекты которые уже произошли не отменяются, например жертва заклинания Слепота не увидит жесты противника которые были выполнены, даже жесты раунда когда снятие чар было скастовано и Слепота пропала. Все эффекты будут отменены без выбора, также Снятие Чар уничтожает вызванного монстра если было скастовано на него, но монстр еще может атаковать в этот раунда. Колдуны не имеют никаких негативных эфектов от заклятия кроме снятия чар.",
                   "Terminates the effects of all Enchantment Spells that have been cast on the subject including those that were cast on the subject in the same turn as the Remove Enchantment.<br>Effects that have already taken place are not cancelled, for example, the victim of a Blindness spell will not be able to see what their opponent's gestures were on the same turn that the Blindness is removed. All enchantments are removed and the caster may not pick and choose. Remove Enchantment also destroys any monster upon which it is cast, although the monster can still attack in that turn. Warlocks suffer no adverse effects from this spell, aside from the removal of their enchantments.");
    fillDictionary("PPws_desc",
                   "Це заклинання подібно Сліпоті тільки ціль закляття стає невидимим для свого противника і його монстрів. Його жести не видно його опонентами, і він не може бути атакований будь-яких монстрів або постраждали від будь-яких цільових конкретних заклинань, хоча вони можуть бути спрямовані на нього на випадок, якщо він стане видимим передчасно. Чаклуни ще можуть завдати удар в тій же надії. Будь монстр який стає невидимим руйнується у зв'язку з нестабільною природи істот створених магією, і не може атакувати.",
                   "Это заклинание подобно Слепоте только цель заклинания становится невидимым для своего противника и его монстров. Его жесты не видно оппонентами, и он не может быть атакован любых монстров или пострадали от любых целевых конкретных заклинаний, хотя они могут быть направлены на него на случай, если он станет видимым преждевременно. Колдуны еще могут нанести удар в той же надежды. Любой монстр который становится невидимым разрушается в связи с нестабильной природы существ созданных магией, и не может атаковать.",
                   "This spell is similar to Blindness only the subject of the spell becomes invisible to his opponent and his monsters. His gestures cannot be seen by his opponents, and he cannot be attacked by any monsters or hit by any target-specific Spells, although they can be directed at him in case he becomes visible prematurely. Warlocks can still stab at him, with the same hope. Any monster made invisible is destroyed due to the unstable nature of such magically created creatures, and doesn't get to attack that turn.");
    fillDictionary("PSDD_desc",
                   "Крім відміни з іншими чарами, це заклинання впливає тільки на монстрів (крім елементалів). Контроль монстра передається заклинателю закляття (або зберігаються за ним), у цьому рунді, тобто монстр буде атакувати того на кого вкаже йому новий хазяїн. В подальшому монстра можна перечаклувати назад. Якщо об'єкт є також ціллю Паралічу, Заплутаності, Зачарування Людини, Зачарування Монстра, Страху або іншої Амнезії то жодне з цих заклять не спрацює.",
                   "Кроме отмены с другими чарами, это заклинание влияет только на монстров (кроме элементалей). Контроль монстра передается заклинателя заклинания (или сохраняются за ним), в этом рунди, есть монстр будет атаковать того на кого укажет ему новый хозяин. В дальнейшем монстра можно околдовать назад. Если объект является также целью Паралича, Запутаности, Очарование Человека, Очарование Монстра, Страха или иной Амнезии то ни одно из этих заклинаний не сработает.",
                   "Except for cancellation with other enchantments, this spell only affects monsters (excluding elementals). Control of the monster is transferred to the caster of the spell (or retained by him) as of this turn, i.e. the monster will attack whosoever its new controller directs, from that turn onwards including that turn. Further charms are, of course, possible, transferring as before. If the subject of the charm is also the subject of any of: Paralysis, Amnesia, Confusion, Charm Person, another Charm Monster, or Fear none of the spells work.");
    fillDictionary("PSDF_desc",
                   "Крім відміни з іншими чарами, це заклинання впливає тільки на чорнокнижників. Ціль закляття вибирає жести як зазвичай, але заклинатель встановлює свій жест для однієї руки зачаклованого. Зачаклований використовує жест, вибраний Чаклуном наступного раунду. Якщо ціль закляття захищена Чарівним Люстерком, то ціль вибирає жести для заклинателя.  Якщо об'єкт є також ціллю Паралічу, Заплутаності, Зачарування Людини, Зачарування Монстра, Страху або іншої Амнезії то жодне з цих заклять не спрацює.",
                   "Кроме отмены с другими чарами, это заклинание влияет только на чернокнижников. Цель заклинания выбирает жесты как обычно, но заклинатель устанавливает свой жест для одной руки околдованого. Заколдованный использует жест, выбранный Колдуном в следующем раунде. Если цель заклятие защищена Волшебным Зеркалом, то цель выбирает жесты для заклинателя. Если объект является также целью Паралича, Запутаности, Очарование Человека, Очарование Монстра, Страха или иной Амнезии то ни одно из этих заклинаний не сработает.",
                   "Except for cancellation with other enchantments, this spell only affects Warlocks. The subject of the spell submits orders as normal, but the caster of the spell also submits a gesture for one of the subject's hands. The subject uses the gesture submitted by the caster for the next turn. If the subject is only so because of a reflection from a Magic Mirror the subject of the mirror assumes the role of caster and decides his opponent's gesture. If the subject is also the subject of any of Paralysis, Amnesia, Confusion, another Charm Person, Charm Monster, or Fear, none of the spells work.");
    fillDictionary("PWPFSSSD_desc",
                   "Ціль закляття стає мертвою мов камінь. Це заклинання настільки сильне, що він його не може зипинити Магічний Захист, хоча Розсіювання Магія виконане в той же хід зупинить.<br>Звичайний спосіб запобігти цього закляттю, щоб запобігти його кастуванню використовуючи, наприклад, Анти Закляття.",
                   "Цель заклинания становится мертвой как камень. Это заклинание настолько сильное, что он его не может остановить Магический Защита, хотя Рассеивание Магия выполнено в тот же ход остановит.<br>Обычный способ предотвращения этого заклятию, чтобы предотвратить его кастуванню используя, например, Анти Заклятие.",
                   "Kills the subject stone dead. This spell is so powerful that it is unaffected by a Counter Spell, although a Dispel Magic spell cast upon the final gesture will stop it.<br>The usual way to prevent being harmed by this spell is to prevent its casting using, for example, an Anti Spell.");
    fillDictionary("PWPWWc_desc",
                   "Протягом наступних 3 ходів, ціль закліття (але не його монстри якщо ціль Чаклун) робить додатковий набір жестів у зв'язку з його прискоренням. Це набуває чинності негайно, відразу після раунду в якому скастоване Прискорення. Всі інші бачать що прискорений Чаклун робить.<br>Зверніть увагу, що будь-яке заклинання яке почало дію в неприскореному раунді, продовжить свою дію у прискореному. Це означає, що (наприклад) Щит навіяний не прискореним Чаклуном також захистити його від удар ножем від прискореного Чаклуна у прискореному раунді.<br>Також зверніть увагу що дія закляття яке почалось у прискореному ході, завершиться у кінці прискореного ходу.<br>Нарешті, зверніть увагу, що чари все ще ефективні під час прискореного раунду, тобто при амнезії прискорений чаклун у прискореному раунді знову повторить свої жести.",
                   "В течение следующих 3 ходов, цель заклиття (но не его монстры если цель Колдун) делает дополнительный набор жестов в связи с его ускорением. Это вступает в силу немедленно, сразу после раунда в котором скастоване Ускорение. Все другие видят что ускоренный Колдун делает.<br>Обратите внимание, что любое заклинание которое начало действие в неускоренной раунде, продолжит свое действие в ускоренном. Это означает, что (например) Щит производимое не ускоренным Колдуном также защитить его от удар ножом от ускоренного Колдуна в ускоренном раунде.<br>Также обратите внимание, что действие заклятия которое началось в ускоренном ходе, завершится в конце ускоренного хода.<br>Наконец, обратите внимание, что волшебство все еще эффективны при ускоренного раунда, т.е. при амнезии ускоренный колдун в ускоренном раунде снова повторит свои жесты.",
                   "For the next 3 turns, the subject (but not his monsters if a Warlock) makes an extra set of gestures due to being speeded up. This takes effect immediately, so that immediately after the turn in which the Haste is cast the Hastened Warlock gets a \"free\" set of orders. Non-hastened Warlocks and monsters can see everything the hastened individual is doing.<br>Note that any spells cast during a non-hastened turn do not expire before the end of the extra hastened turn. This means that (for example) a Shield cast by a non-hastened Warlock would also protect him against a stab from a hastened Warlock in the hastened Warlock's free turn.<br>Note also that lasting spells cast during the hastened turn will begin to time out before the next turn. For example, a Shield cast during the free turn is not effective in the next.<br>Finally, note that enchantments are still effective during a Hasted turn, but, if the caster doesn't submit orders that turn, and the enchantment requires direction (as Paralysis and Charm Person do), then it will not take effect.");
    fillDictionary("SD_desc",
                   "Це заклинання створює матеріальний об'єкт твердої речовини, яка летить у бік суб'єкта заклинання і спричиняє один пункт ушкодження. Від заклинання можна захиститись Щитом.",
                   "Это заклинание создает материальный объект твердого вещества, которая летит в сторону субъекта заклинание и вызывает один пункт повреждения. От заклинания можно защититься Щитом.",
                   "This spell creates a material object of hard substance which is hurled towards the subject of the spell and causes him one point of damage. The spell is thwarted by a Shield.");
    fillDictionary("SPFP_desc",
                   "Наступного ходу ціль заклинання, не зможе використовувати жести, зроблені у попередніх раунда для побудови заклять. Заклинання не впливає на заклинання, які застосовані в цьому раунді і не впливають на монстрів.",
                   "Следующего хода цель заклинание, не сможет использовать жесты, сделанные в предыдущих раунда для построения заклинаний. Заклинание не влияет на заклинания, которые применены в этом раунде и не влияют на монстров.",
                   "On the turn following the casting of this spell, the subject cannot include any gestures made on or before this turn in a spell sequence and must restart a new spell from the beginning of that spell sequence. The spell does not affect spells which are cast on the same turn nor does it affect monsters.");
    fillDictionary("SPFPSDW_desc",
                   "Це заклинання працює тільки тоді, коли накладається на Чаклуна. Будь-яке заклинання він завершує (тобто завершує жести і кидає), за умови, що на одному з найближчих 3 ходу, і які потрапляє в категорію «чари» (за винятком Анти Чари, Хвороба, Отрута, або Time Stop) стає постійним.",
                   "Это заклинание работает только тогда, когда накладывается на Колдуна. Любое заклинание он завершает (т.е. завершает жесты и бросает), при условии, что на одном из ближайших 3 хода, и которые попадает в категорию «чары» (за исключением Анти Чары, Болезнь, Яд, или Time Stop) становится постоянным",
                   "This spell only works if cast upon a Warlock. Any spell he completes (i.e. completes the gestures of and casts), provided it is on one of the next 3 turns, and which falls into the category of \"Enchantments\" (except Anti Spell, Disease, Poison, or Time Stop) can have its effect made permanent. Delay Effect and Permanency can be made permanent, which prevents the three turn timeout, but they are still used up after being triggered. Note that the person who has his spell made permanent does not necessarily have to make himself the subject of the spell. A Permanency spell cannot increase the duration of a spell which is being banked by Delay Effect. Note that an attempt to make permanent an enchantment spell that is countered (or fails for any other reason) still uses up the Permanency. Note: Cast Permanency at yourself! Permanency doesn't work with Charm Monster, as that spell has an instant effect rather than actually behaving as an enchantment.");
    fillDictionary("SPPc_desc",
                   "Ціль цього закляття отримує додатковий хід відразу ж по закінченні поточного раунду, якщо дуєль ще не закінчилась. В ході додаткового рукнду ніхто не бачить жести Чаклуна. Також ніхто немає захисту від атак проведених під часс цього додаткового раунду.",
                   "Цель этого заклинания получает дополнительный ход сразу же по окончании текущего раунда, если дуель еще не закончился. В ходе дополнительного рукнду никто не видит жесты Колдуна. Также никто не имеет защиты от атак проведенных под часс этого дополнительного раунда.",
                   "The subject of this spell takes an extra turn immediately at the end of the current turn, assuming they are still alive. During the extra turn, no-one can see the gestures he makes (though they can detect the effects of spells he casts). All beings have no resistance to any form of attack during a Time-Stopped turn, e.g. a Warlock halfway through the duration of a Protection spell can be harmed by a monster which has had its time stopped. Time stopped monsters attack whoever they were attacking in the round Time Stop is cast, and time stopped elementals affect everyone, resistance to heat or cold being immaterial. Also, enchantments which affect a Warlock's gestures are not effective during the time-stopped turn (though they still take effect afterwards). It's possible, therefore, to negate an enchantment by casting another on yourself during a time-stopped turn. Essentially, no persistent effects are relevant in a time-stopped turn. Also, a counter-spell or shield cast during a time-stopped turn will not last into the next turn. A time-stopped monster will attack before a surrender is recognised, though a time-stopped player will not. Finally, if two Warlocks are both time-stopped in the same turn, they can both see the other.");
    fillDictionary("SPPFD_desc",
                   "Ціль цього закляття отримує додатковий хід відразу ж по закінченні поточного раунду, якщо дуєль ще не закінчилась. В ході додаткового рукнду ніхто не бачить жести Чаклуна. Також ніхто немає захисту від атак проведених під часс цього додаткового раунду.",
                   "Цель этого заклинания получает дополнительный ход сразу же по окончании текущего раунда, если дуель еще не закончился. В ходе дополнительного рукнду никто не видит жесты Колдуна. Также никто не имеет защиты от атак проведенных под часс этого дополнительного раунда.",
                   "The subject of this spell takes an extra turn immediately at the end of the current turn, assuming they are still alive. During the extra turn, no-one can see the gestures he makes (though they can detect the effects of spells he casts). All beings have no resistance to any form of attack during a Time-Stopped turn, e.g. a Warlock halfway through the duration of a Protection spell can be harmed by a monster which has had its time stopped. Time stopped monsters attack whoever they were attacking in the round Time Stop is cast, and time stopped elementals affect everyone, resistance to heat or cold being immaterial. Also, enchantments which affect a Warlock's gestures are not effective during the time-stopped turn (though they still take effect afterwards). It's possible, therefore, to negate an enchantment by casting another on yourself during a time-stopped turn. Essentially, no persistent effects are relevant in a time-stopped turn. Also, a counter-spell or shield cast during a time-stopped turn will not last into the next turn. A time-stopped monster will attack before a surrender is recognised, though a time-stopped player will not. Finally, if two Warlocks are both time-stopped in the same turn, they can both see the other.");
    fillDictionary("SSFP_desc",
                   "Ефект цього закляття ідентичний Захисту від Жару, але захищаэ выд Холоду (Крижаний Шторм та Крижаний Елементаль), воно руйнує крижаних елементалів, якщо вони є цыллю заклинання, але не впливає на вогненних елементалів.",
                   "Эффект этого заклинания идентичен Защиты от Жара, но захищаэ выд Холода (Ледяной Шторм и Ледяной Елементаль), оно разрушает ледяных элементалей, если они цыллю заклинание, но не влияет на огненных элементалей.",
                   "The effects of this spell are identical to Resist Heat but resistance is to cold (Ice Storm and ice elementals) and it destroys ice elementals if they are the subject of the spell but doesn't affect fire elementals.");
    fillDictionary("SWD_desc",
                   "Якщо суб'єкт цього заклинання Чернокнижник, то у наступному раунді, зачарованийй може не виконати жести C, D, F S (якщо вони будуть замінені на -).<br>Якщо предметом заклинання це чудовисько (за винятком елементалів), то воно буде занадто налякане, щоб атакувати в цьому раунді.<br>Якщо об'єкт є також ціллю Паралічу, Заплутаності, Зачарування Людини, Зачарування Монстра, Страху або іншої Амнезії то жодне з цих заклять не спрацює.",
                   "Если субъект этого заклинания Чернокнижник, то в следующем раунде, зачарованийй может не выполнить жесты C, D, FS (если они будут заменены на -).<br>Если предметом заклинание это чудовище (за исключением элементалей), то оно будет слишком напугано, чтобы атаковать в этом раунде.<br>Если объект является также целью Паралича, Запутаности, Очарование Человека, Очарование Монстра, Страха или иной Амнезии то ни одно из этих заклинаний не сработает.",
                   "If the subject of this spell is a Warlock, then on the turn following the casting of this spell, the subject cannot perform a C, D, F or S gesture (if they submit orders to make one of these gestures, their gesture will be replaced with '-' - no gesture).<br>If the subject of the spell is a monster (excluding elementals, which are unaffected), it will be too afraid to attack in that round.<br>If the subject is also the subject of Paralysis, Amnesia, Confusion, Charm Person, Charm Monster, or another Fear, then none of the spells work.");
    fillDictionary("SWWc_desc",
                   "Кожен, хто не стійкій до жару отримує 5 пунктів ушкодження. Заклинання скасовується повністю, не завдаючи шкоди, або з Крижаним Штормом або Крижаним Елементалем. Вогнянний Шторм знищить також Вогненого елементаля, але також завдасть шкоді всім іншим як звичайно. Два  Шторми будуть діяти як один. Магічний захист не зупинить Пекельний Шторм, але захистить власника.",
                   "Каждый, кто не устойчивой к жару получает 5 пунктов повреждения. Заклинание отменяется полностью, не причиняя вреда, или Ледяным штормом или Ледяным элементалем. Огненных Шторм уничтожит также огненные элементаля, но также нанесет вред всем остальным как обычно. Два Штормы будут действовать как один. Магическая защита не остановит Огненный Шторм, но защитит владельца.",
                   "Everything not resistant to heat sustains 5 points of damage that turn. The spell cancels wholly, causing no damage, with either an Ice Storm or an ice elemental. It will destroy but not be destroyed by a fire elemental. Two Fire Storms act as one. A Counter Spell will not prevent a Fire Storm, but it will protect the target of the Counter Spell.");
    fillDictionary("WDDc_desc",
                   "Аналогічно до Блискавки, але це закляття Чаклун може використати тільки раз в битві. (Примітка - якщо закляття заблоковане контр-заклинання, то не може бути використане знову, але якщо закляття зупинене Розсіюванням магії, то може бути використане знову)",
                   "Аналогично Молнии, но это заклятие Колдун может использовать только раз в битве. (Примечание - если заклятие заблокировано контр-заклинание, не может быть использовано снова, но если заклятие остановлено Рассеиванием магии, то может быть использовано снова)",
                   "The subject of this spell is hit by a bolt of lightning and sustains five points of damage. A Shield spell offers no defence. This spell may only be cast once per Warlock in any one Battle. (Note - if it is blocked by a counter-spell, it still may not be cast again. If it is prevented by Dispel Magic, it can be cast again.)");
    fillDictionary("WFP_desc",
                   "Ціль закляття отримує 2 очка ушкодження. Щит не дає захисту.",
                   "Цель заклинания получает 2 очка повреждения. Щит не дает защиты.",
                   "The subject of this spell is inflicted with two points of damage. A Shield spell offers no defence.");
    fillDictionary("WPFD_desc",
                   "Таке ж як Нанести Легкі Рани, але завдає три очка ушкодження замість двох.",
                   "Такое же как Нанести Легкие Раны, но наносит три очка повреждения вместо двух.",
                   "This has the same effect as Cause Light Wounds but inflicts three points of damage instead of two.");
    fillDictionary("WPP_desc",
                   "Будь-яке інше заклинання націлене на ціль захищену Магічним Захистом не буде мати ніякого ефекту. Магічний захист також зберігає від впливу заклятть які діють більш ніж на одну ціль, наприклад Пекельний Шторм. Магічний захист знешкоджує всі закляття крім Розсіяння Магії та Палець Смерті, а також працює в якості щита для своєї цілі.",
                   "Любое другое заклинание нацелено на цель защищенную Магическим защиты не будет иметь никакого эффекта. Магическая защита также сохраняет от воздействия заклинаний действующих более чем на одну цель, например Огненный Шторм. Магическую защиту обезвреживает все заклинания кроме Рассеяние Магии и Пальца Смерти, а также работает в качестве щита для своей цели.",
                   "Any other spell cast upon the subject in the same turn has no effect. In the case of blanket spells, which affect more than one person, the subject of the Counter Spell alone is protected. For example, a Fire Storm spell could kill off a monster but not if a Counter Spell were cast on the monster in the same turn. Everyone else would be affected as usual by the Fire Storm unless they had their own protection. Counter Spell works on all spells except Dispel Magic and Finger of Death, and also works as a Shield for its Target.");
    fillDictionary("WSSc_desc",
                   "Кожен, хто не стійкий до холоду отримає 5 пунктів ушкодження. Заклинання скасовує повністю, не завдаючи шкоди, або Пекельним Штормом або Вогняним Елементалем, і скасується локально Вогняною Кулею. Вогняний Шторм знищить, але не бути знищеньй Крижаним Елементалем. Два Крижаних Шторма будуть діяти як один. Магічний захист не зупинить Крижаний Шторм, але захистить власника.",
                   "Каждый, кто не устойчив к холоду получит 5 пунктов повреждения. Заклинание отменяет полностью, не причиняя вреда, или Адским Штормом или Огненным элементалей, и упразднится локально Огненной Пулей. Огненный Шторм уничтожит, но не быть знищеньй Ледяным элементалей. Два Ледяных Шторма будут действовать как один. Магическую защиту не остановит Ледяной Шторм, но защитит владельца.",
                   "Everything not resistant to cold sustains 5 points of damage that turn. The spell cancels wholly, causing no damage, with either a Fire Storm or a fire elemental, and will cancel locally with a Fireball. It will destroy but not be destroyed by an ice elemental. Two Ice Storms act as one. A Counter Spell will not prevent an Ice Storm, but it will protect the target of the Counter Spell.");
    fillDictionary("WWFP_desc",
                   "Ціль закляття стає повністю стійка до всіх форм атаки Жаром (Вогненна Куля, Пекельний Шторм і Вогняний Елементаль). Тільки Розсіювання Магія і Зняття Чар припинять дію цього захисту (Магічний захист поглине це закляття якщо бути задіяне в той же раунд що і Захист выд Жару). Захист від Жару знищить Вогенного Елементаля, перш ніж він може атакувати, але не вплине на крижаних елементалів.",
                   "Цель заклинания становится полностью устойчива ко всем формам атаки Жаром (Огненный Шар, Огненный Шторм и Огненный Елементаль). Только Рассеивание Магия и Снятие Чар прекратят действие защиты (Магическая защита поглотит это заклятие если будет задействовано в том же раунд что и Защита от Жара). Защита от Жара уничтожит Огенного элементаля, прежде чем он может атаковать, но не повлияет на ледяных элементалей.",
                   "The subject of this spell becomes totally resistant to all forms of heat attack (Fireball, Fire Storm and fire elementals). Only Dispel Magic or Remove Enchantment will terminate this resistance once started (although a Counter Spell will prevent it from working if cast at the subject at the same time as this spell). A Resist Heat cast directly on a fire elemental will destroy it before it can attack that turn, but there is no effect on ice elementals.");
    fillDictionary("WWP_desc",
                   "Аналогічно Щиту, тільки продовжуеться 3 раунди (раунд в якому скастоване + ще 2).",
                   "Аналогично Щиту, только продолжаеться 3 раунда (раунд в котором скастовано + еще 2).",
                   "For this turn and the following two turns the subject of this spell is protected as if using a Shield spell, thus leaving both hands free. Concurrent Shield spells offer no further protection and compound Protection spells merely overlap offering no extra cover.");
    fillDictionary("WWS_desc",
                   "Будь-яке інше заклинання націлене на ціль захищену Магічним Захистом не буде мати ніякого ефекту. Магічний захист також зберігає від впливу заклятть які діють більш ніж на одну ціль, наприклад Пекельний Шторм. Магічний захист знешкоджує всі закляття крім Розсіяння Магії та Палець Смерті, а також працює в якості щита для своєї цілі.",
                   "Любое другое заклинание нацелено на цель защищенную Магическим защиты не будет иметь никакого эффекта. Магическая защита также сохраняет от воздействия заклинаний действующих более чем на одну цель, например Огненный Шторм. Магическую защиту обезвреживает все заклинания кроме Рассеяние Магии и Пальца Смерти, а также работает в качестве щита для своей цели.",
                   "Any other spell cast upon the subject in the same turn has no effect. In the case of blanket spells, which affect more than one person, the subject of the Counter Spell alone is protected. For example, a Fire Storm spell could kill off a monster but not if a Counter Spell were cast on the monster in the same turn. Everyone else would be affected as usual by the Fire Storm unless they had their own protection. Counter Spell works on all spells except Dispel Magic and Finger of Death, and also works as a Shield for its Target.");
    fillDictionary(">_desc",
                   "Удар ножем, наносить 1 очко",
                   "Удар ножом, наносит 1 очко урона",
                   "This is not a spell but an attack which can be directed at any individual monster or Warlock (no spell need be selected, but a target should be, unless you want to attack a random opposing Warlock). Unless protected in that turn by a Shield spell or another spell with the same effect, the stabbed being suffers 1 point of damage. The Warlock only has one knife so can only stab with one hand in any turn - if a Warlock's gestures are such that he stabs with both hands, only the right hand will do damage. The stab cannot be reflected by a Magic Mirror or stopped by Dispel Magic (although its Shield effect *could* stop the stab). Clumsy Warlocks are allowed to stab themselves. Knives cannot be thrown. ");
}

void QWarlockDictionary::storeLang() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Locale");
    settings.setValue("language", lang);
    settings.endGroup();
}

QString QWarlockDictionary::getLang() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Locale");
    lang = settings.value("language", "en").toString();
    settings.endGroup();

    return lang;
}

QGameDictionary * QWarlockDictionary::getInstance() {
    //qDebug() << "QWarlockDictionary::getInstance";
    if (!self) {
        self = new QWarlockDictionary();
        static_cast<QWarlockDictionary *>(self)->fillGameDictionary();
        static_cast<QWarlockDictionary *>(self)->getLang();
    }
    return self;
}
