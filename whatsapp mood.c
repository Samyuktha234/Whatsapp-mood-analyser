/*
 =====================================================
   WhatsApp Chat Sentiment & Mood Analyzer
   "Decode the Mood Behind the Message"
 =====================================================
   HOW TO RUN (Turbo C++):
   1. Open Turbo C++
   2. File -> New
   3. Paste this entire code
   4. File -> Save As -> mood_analyzer.c
   5. Compile: Alt + F9
   6. Run:     Ctrl + F9

   HOW TO RUN (Dev C++ / Code::Blocks / VS Code):
   1. Paste code into a new file -> save as mood_analyzer.c
   2. Compile and Run (F9 in Dev C++)
 =====================================================
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* --- Constants ----------------------------------- */
#define MAX_NAME      50
#define MAX_MSG      200
#define MAX_PEOPLE    20
#define MAX_MESSAGES 500
#define MAX_LINE     300

/* --- Mood Values --------------------------------- */
#define HAPPY    0
#define SAD      1
#define ANGRY    2
#define LOVE     3
#define NEUTRAL  4

/* --- Structs ------------------------------------- */
struct Message {
    char sender[MAX_NAME];
    char message[MAX_MSG];
    char date[15];
    int  mood;
};

struct Person {
    char name[MAX_NAME];
    int  total;
    int  happy;
    int  sad;
    int  angry;
    int  love;
    int  neutral;
};

/* --- Global Data --------------------------------- */
struct Message messages[MAX_MESSAGES];
struct Person  people[MAX_PEOPLE];
int total_messages = 0;
int total_people   = 0;
int data_loaded    = 0;

/* ---------------------------------------------------
   MOOD KEYWORD LISTS
   --------------------------------------------------- */

const char *happy_words[] = {
    "haha","lol","lmao","hehe","great","awesome","happy",
    "wonderful","fantastic","amazing","yay","congrats",
    "perfect","enjoy","fun","best","excited","glad","wow",
    "brilliant","nice","good","superb","excellent","cool",
    "sweet","wohoo","yayyy","laugh","smile","proud","blessed",
    "grateful","thankful","cheerful","thrilled","delighted",
    "celebrate","hooray","wonderful","winning","cheers",
    NULL
};

const char *sad_words[] = {
    "sad","cry","miss","alone","hurt","pain","sorry","upset",
    "depressed","lonely","crying","unhappy","terrible","awful",
    "disappointed","grief","lost","hopeless","tears","broken",
    "suffer","miserable","sorrow","regret","tired","exhausted",
    "weak","helpless","empty","leaving","goodbye","low","down",
    "heartbreak","devastated","gloomy","mourn","weep",
    NULL
};

const char *angry_words[] = {
    "hate","angry","stupid","idiot","worst","annoying","mad",
    "furious","disgusting","pathetic","useless","frustrated",
    "irritated","rude","horrible","nonsense","rubbish","trash",
    "seriously","ugh","argh","unfair","wrong","liar","fake",
    "cheat","selfish","arrogant","ridiculous","enough","shut",
    "fool","dumb","irritating","fed up","sick of",
    NULL
};

const char *love_words[] = {
    "love","cute","sweetheart","darling","adore","beautiful",
    "heart","kiss","hug","dear","precious","romantic","babe",
    "honey","care","crush","forever","always","together",
    "special","angel","baby","sunshine","ily","luv","xoxo",
    "mwah","muah","bestie","buddy","bestfriend","soulmate",
    "cherish","affection","devoted","miss you","thinking of you",
    NULL
};

/* ---------------------------------------------------
   UTILITY FUNCTIONS
   --------------------------------------------------- */

void str_lower(const char *src, char *dst) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = tolower((unsigned char)src[i]);
        i++;
    }
    dst[i] = '\0';
}

int count_hits(const char *message, const char * const *words) {
    char lower[MAX_MSG];
    int  count = 0, k = 0;
    str_lower(message, lower);
    while (words[k] != NULL) {
        if (strstr(lower, words[k]) != NULL)
            count++;
        k++;
    }
    return count;
}

int detect_mood(const char *message) {
    int h = count_hits(message, happy_words);
    int s = count_hits(message, sad_words);
    int a = count_hits(message, angry_words);
    int l = count_hits(message, love_words);
    int mx = 0, mood = NEUTRAL;
    if (h > mx) { mx = h; mood = HAPPY;  }
    if (s > mx) { mx = s; mood = SAD;    }
    if (a > mx) { mx = a; mood = ANGRY;  }
    if (l > mx) { mx = l; mood = LOVE;   }
    return mood;
}

const char *mood_name(int mood) {
    if (mood == HAPPY)   return "HAPPY   :) ";
    if (mood == SAD)     return "SAD     :( ";
    if (mood == ANGRY)   return "ANGRY   >( ";
    if (mood == LOVE)    return "LOVE    <3 ";
    return                      "NEUTRAL :| ";
}

const char *get_label(struct Person *p) {
    int t = p->total;
    int h, s, a, l;
    if (t == 0) return "Unknown";
    h = p->happy   * 100 / t;
    s = p->sad     * 100 / t;
    a = p->angry   * 100 / t;
    l = p->love    * 100 / t;
    if (h >= 40) return "*** The Cheerful One  :)  ***";
    if (s >= 35) return "*** The Emotional One :(  ***";
    if (a >= 35) return "*** The Fiery One     >(  ***";
    if (l >= 35) return "*** The Romantic One  <3  ***";
    return               "*** The Chill One     :|  ***";
}

void print_bar(const char *label, int pct) {
    int i;
    int filled = pct / 5;
    printf("  %-8s [", label);
    for (i = 0; i < 20; i++)
        printf(i < filled ? "#" : ".");
    printf("] %3d%%\n", pct);
}

void register_sender(const char *name, int mood) {
    int i;
    for (i = 0; i < total_people; i++) {
        if (strcmp(people[i].name, name) == 0) {
            people[i].total++;
            if      (mood == HAPPY)   people[i].happy++;
            else if (mood == SAD)     people[i].sad++;
            else if (mood == ANGRY)   people[i].angry++;
            else if (mood == LOVE)    people[i].love++;
            else                      people[i].neutral++;
            return;
        }
    }
    if (total_people < MAX_PEOPLE) {
        strcpy(people[total_people].name, name);
        people[total_people].total   = 1;
        people[total_people].happy   = (mood == HAPPY)   ? 1 : 0;
        people[total_people].sad     = (mood == SAD)     ? 1 : 0;
        people[total_people].angry   = (mood == ANGRY)   ? 1 : 0;
        people[total_people].love    = (mood == LOVE)    ? 1 : 0;
        people[total_people].neutral = (mood == NEUTRAL) ? 1 : 0;
        total_people++;
    }
}

void clear_screen() {
    int i;
    for (i = 0; i < 50; i++) printf("\n");
}

void pause_screen() {
    printf("\n  Press ENTER to go back to menu...");
    fflush(stdin);
    getchar();
}

/* ---------------------------------------------------
   OPTION 1 : LOAD WHATSAPP CHAT FROM FILE
   ---------------------------------------------------
   WhatsApp Android export format:
   19/04/2026, 10:30 am - Rahul: Hey how are you?

   HOW TO GET YOUR CHAT FILE:
   1. Open WhatsApp -> Open any chat
   2. Tap 3 dots (top right) -> More -> Export Chat
   3. Select "Without Media"
   4. Save/Share the .txt file to your PC
   5. Put it in the SAME folder as mood_analyzer.c
   6. Then choose option 1 and type the filename
   --------------------------------------------------- */
void load_chat_file() {
    FILE *fp;
    char  filename[100];
    char  line[MAX_LINE];
    char *dash, *colon;
    int   name_len, loaded = 0;
    struct Message *msg;

    clear_screen();
    printf("=====================================================\n");
    printf("         LOAD WHATSAPP CHAT FROM FILE\n");
    printf("=====================================================\n");
    printf("  Make sure your chat .txt file is in the\n");
    printf("  SAME folder as mood_analyzer.c\n");
    printf("-----------------------------------------------------\n");
    printf("  Enter filename (e.g. chat.txt): ");
    scanf("%99s", filename);
    getchar();

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\n  ERROR: Cannot open '%s'\n", filename);
        printf("  Check: Is the file in the same folder?\n");
        printf("  Check: Did you spell the filename correctly?\n");
        pause_screen();
        return;
    }

    /* Reset old data */
    total_messages = 0;
    total_people   = 0;

    while (fgets(line, MAX_LINE, fp) != NULL) {
        /* Skip WhatsApp system lines */
        if (strstr(line, " - ")    == NULL) continue;
        if (strstr(line, ": ")     == NULL) continue;
        if (strstr(line, "omitted"))        continue;
        if (strstr(line, "end-to-end"))     continue;
        if (strstr(line, "Messages and calls")) continue;

        if (total_messages >= MAX_MESSAGES) break;

        msg = &messages[total_messages];

        /* Extract date: first 10 characters */
        strncpy(msg->date, line, 10);
        msg->date[10] = '\0';

        /* Find sender name: text between " - " and ": " */
        dash = strstr(line, " - ");
        if (dash == NULL) continue;
        dash += 3;

        colon = strstr(dash, ": ");
        if (colon == NULL) continue;

        name_len = (int)(colon - dash);
        if (name_len <= 0 || name_len >= MAX_NAME) continue;

        strncpy(msg->sender, dash, name_len);
        msg->sender[name_len] = '\0';

        /* Extract message text: after ": " */
        strncpy(msg->message, colon + 2, MAX_MSG - 1);
        msg->message[MAX_MSG - 1] = '\0';
        msg->message[strcspn(msg->message, "\n\r")] = '\0';

        /* Detect mood and register sender */
        msg->mood = detect_mood(msg->message);
        register_sender(msg->sender, msg->mood);

        total_messages++;
        loaded++;
    }

    fclose(fp);

    if (loaded > 0) {
        data_loaded = 1;
        printf("\n  SUCCESS! File loaded.\n");
        printf("  Messages read  : %d\n", total_messages);
        printf("  People found   : %d\n", total_people);
    } else {
        printf("\n  No valid messages found in the file.\n");
        printf("  Make sure it is a WhatsApp exported chat .txt\n");
    }
    pause_screen();
}

/* ---------------------------------------------------
   OPTION 2 : TYPE A MESSAGE MANUALLY
   --------------------------------------------------- */
void type_message_manually() {
    char sender[MAX_NAME];
    char msg_text[MAX_MSG];
    int  mood;
    struct Message *msg;

    clear_screen();
    printf("=====================================================\n");
    printf("          TYPE A MESSAGE MANUALLY\n");
    printf("=====================================================\n");

    if (total_messages >= MAX_MESSAGES) {
        printf("  Message storage is full (max %d).\n", MAX_MESSAGES);
        pause_screen();
        return;
    }

    printf("  Enter sender name : ");
    fgets(sender, MAX_NAME, stdin);
    sender[strcspn(sender, "\n\r")] = '\0';

    printf("  Enter message     : ");
    fgets(msg_text, MAX_MSG, stdin);
    msg_text[strcspn(msg_text, "\n\r")] = '\0';

    if (strlen(sender) == 0 || strlen(msg_text) == 0) {
        printf("\n  Name or message cannot be empty.\n");
        pause_screen();
        return;
    }

    mood = detect_mood(msg_text);
    msg  = &messages[total_messages];

    strcpy(msg->sender,  sender);
    strcpy(msg->message, msg_text);
    strcpy(msg->date,    "Manual");
    msg->mood = mood;

    register_sender(msg->sender, mood);
    total_messages++;
    data_loaded = 1;

    printf("\n-----------------------------------------------------\n");
    printf("  Message added!\n");
    printf("  Sender : %s\n", sender);
    printf("  Mood   : %s\n", mood_name(mood));
    printf("-----------------------------------------------------\n");
    pause_screen();
}

/* ---------------------------------------------------
   OPTION 3 : DEMO MODE
   --------------------------------------------------- */
void load_demo() {
    int i;
    const char *demo_sender[] = {
        "Rahul","Priya","Rahul","Priya","Rahul",
        "Priya","Rahul","Priya","Rahul","Priya",
        "Rahul","Priya","Rahul","Priya","Rahul",
        "Priya","Rahul","Priya","Rahul","Priya"
    };
    const char *demo_msg[] = {
        "haha that was so funny lol amazing",
        "I love you so much babe you are my heart",
        "love you too sweetheart you are cute",
        "I miss you so much I am so sad and lonely",
        "aww dont cry everything will be great awesome",
        "you are the best person ever so brilliant",
        "that is so stupid and annoying seriously ugh",
        "why are you angry stop it please calm down",
        "haha just kidding lol I am happy and excited",
        "okay hehe you are cute I adore you forever",
        "okay sounds good perfect plan wow",
        "I am so alone and lonely today feeling low",
        "wow that is so awesome brilliant superb",
        "yes I am so excited yay congrats hooray",
        "miss you so much it hurts I am sad",
        "same miss you darling my heart xoxo love",
        "this is the worst thing ever I hate it mad",
        "haha lol that made me laugh so hard yay",
        "I am so grateful and blessed thankful wow",
        "together forever my love always and always"
    };
    const char *demo_date = "19/04/2026";

    clear_screen();
    total_messages = 0;
    total_people   = 0;

    for (i = 0; i < 20; i++) {
        strcpy(messages[i].sender,  demo_sender[i]);
        strcpy(messages[i].date,    demo_date);
        strcpy(messages[i].message, demo_msg[i]);
        messages[i].mood = detect_mood(demo_msg[i]);
        register_sender(demo_sender[i], messages[i].mood);
        total_messages++;
    }

    data_loaded = 1;
    printf("  DEMO MODE loaded!\n");
    printf("  Messages : %d\n", total_messages);
    printf("  People   : %d  (Rahul & Priya)\n", total_people);
    pause_screen();
}

/* ---------------------------------------------------
   OPTION 4 : MOOD REPORT PER PERSON
   --------------------------------------------------- */
void show_mood_report() {
    int i, t;
    struct Person *p;

    clear_screen();
    printf("=====================================================\n");
    printf("      WHATSAPP CHAT MOOD ANALYSIS REPORT\n");
    printf("     Decode the Mood Behind the Message\n");
    printf("=====================================================\n");
    printf("  Total Messages : %d\n", total_messages);
    printf("  Total People   : %d\n", total_people);
    printf("=====================================================\n");

    for (i = 0; i < total_people; i++) {
        p = &people[i];
        t = p->total;
        if (t == 0) continue;

        printf("\n-----------------------------------------------------\n");
        printf("  Person   : %s\n",   p->name);
        printf("  Label    : %s\n",   get_label(p));
        printf("  Messages : %d\n",   t);
        printf("-----------------------------------------------------\n");
        print_bar("Happy",   p->happy   * 100 / t);
        print_bar("Sad",     p->sad     * 100 / t);
        print_bar("Angry",   p->angry   * 100 / t);
        print_bar("Love",    p->love    * 100 / t);
        print_bar("Neutral", p->neutral * 100 / t);
    }
    printf("\n=====================================================\n");
    pause_screen();
}

/* ---------------------------------------------------
   OPTION 5 : ALL MESSAGES WITH MOOD
   --------------------------------------------------- */
void show_all_messages() {
    int i;
    clear_screen();
    printf("=====================================================\n");
    printf("          ALL MESSAGES WITH MOOD LABEL\n");
    printf("=====================================================\n");
    printf("%-12s %-16s %-12s %s\n",
           "Date", "Sender", "Mood", "Message (preview)");
    printf("-----------------------------------------------------\n");

    for (i = 0; i < total_messages; i++) {
        char preview[41];
        strncpy(preview, messages[i].message, 40);
        preview[40] = '\0';
        printf("%-12s %-16s %-12s %s\n",
               messages[i].date,
               messages[i].sender,
               mood_name(messages[i].mood),
               preview);
    }
    printf("=====================================================\n");
    pause_screen();
}

/* ---------------------------------------------------
   OPTION 6 : CHAT STATISTICS
   --------------------------------------------------- */
void show_statistics() {
    int i;
    int total_h = 0, total_s = 0, total_a = 0, total_l = 0, total_n = 0;
    int max_msgs = 0;
    char active_person[MAX_NAME];

    clear_screen();
    printf("=====================================================\n");
    printf("                 CHAT STATISTICS\n");
    printf("=====================================================\n");

    strcpy(active_person, "Unknown");
    for (i = 0; i < total_people; i++) {
        if (people[i].total > max_msgs) {
            max_msgs = people[i].total;
            strcpy(active_person, people[i].name);
        }
        total_h += people[i].happy;
        total_s += people[i].sad;
        total_a += people[i].angry;
        total_l += people[i].love;
        total_n += people[i].neutral;
    }

    printf("  Total Messages     : %d\n", total_messages);
    printf("  Total People       : %d\n\n", total_people);
    printf("  Most Active Person : %s (%d messages)\n\n",
           active_person, max_msgs);

    printf("  --- Overall Mood Breakdown ---\n");
    if (total_messages > 0) {
        print_bar("Happy",   total_h * 100 / total_messages);
        print_bar("Sad",     total_s * 100 / total_messages);
        print_bar("Angry",   total_a * 100 / total_messages);
        print_bar("Love",    total_l * 100 / total_messages);
        print_bar("Neutral", total_n * 100 / total_messages);
    }

    printf("\n  --- Per Person Message Count ---\n");
    for (i = 0; i < total_people; i++) {
        int t = people[i].total;
        printf("  %-20s : %d messages\n", people[i].name, t);
        printf("             Happy:%d  Sad:%d  Angry:%d  Love:%d  Neutral:%d\n",
               people[i].happy, people[i].sad,
               people[i].angry, people[i].love, people[i].neutral);
    }
    printf("=====================================================\n");
    pause_screen();
}

/* ---------------------------------------------------
   OPTION 7 : SEARCH BY MOOD
   (NEW FEATURE - Show only messages of one mood)
   --------------------------------------------------- */
void search_by_mood() {
    int i, choice, target_mood, found = 0;

    clear_screen();
    printf("=====================================================\n");
    printf("              SEARCH MESSAGES BY MOOD\n");
    printf("=====================================================\n");
    printf("  Which mood do you want to see?\n\n");
    printf("  [1] HAPPY   :)\n");
    printf("  [2] SAD     :(\n");
    printf("  [3] ANGRY   >(\n");
    printf("  [4] LOVE    <3\n");
    printf("  [5] NEUTRAL :|\n");
    printf("-----------------------------------------------------\n");
    printf("  Enter choice (1-5): ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > 5) {
        printf("\n  Invalid choice.\n");
        pause_screen();
        return;
    }
    target_mood = choice - 1;

    clear_screen();
    printf("=====================================================\n");
    printf("  Messages with mood: %s\n", mood_name(target_mood));
    printf("=====================================================\n");
    printf("%-12s %-16s %s\n", "Date", "Sender", "Message");
    printf("-----------------------------------------------------\n");

    for (i = 0; i < total_messages; i++) {
        if (messages[i].mood == target_mood) {
            printf("%-12s %-16s %s\n",
                   messages[i].date,
                   messages[i].sender,
                   messages[i].message);
            found++;
        }
    }

    if (found == 0)
        printf("  No messages found for this mood.\n");

    printf("-----------------------------------------------------\n");
    printf("  Total found: %d message(s)\n", found);
    printf("=====================================================\n");
    pause_screen();
}

/* ---------------------------------------------------
   OPTION 8 : SAVE REPORT TO FILE
   --------------------------------------------------- */
void save_report() {
    FILE *fp;
    int   i, t;
    struct Person *p;

    fp = fopen("mood_report.txt", "w");
    if (fp == NULL) {
        printf("\n  ERROR: Cannot create mood_report.txt\n");
        pause_screen();
        return;
    }

    fprintf(fp, "=====================================================\n");
    fprintf(fp, "      WHATSAPP CHAT MOOD ANALYSIS REPORT\n");
    fprintf(fp, "     Decode the Mood Behind the Message\n");
    fprintf(fp, "=====================================================\n");
    fprintf(fp, "Total Messages : %d\n", total_messages);
    fprintf(fp, "Total People   : %d\n\n", total_people);

    for (i = 0; i < total_people; i++) {
        p = &people[i];
        t = p->total;
        if (t == 0) continue;
        fprintf(fp, "-----------------------------------------------------\n");
        fprintf(fp, "Person   : %s\n",    p->name);
        fprintf(fp, "Label    : %s\n",    get_label(p));
        fprintf(fp, "Messages : %d\n",    t);
        fprintf(fp, "Happy    : %d%%\n",  p->happy   * 100 / t);
        fprintf(fp, "Sad      : %d%%\n",  p->sad     * 100 / t);
        fprintf(fp, "Angry    : %d%%\n",  p->angry   * 100 / t);
        fprintf(fp, "Love     : %d%%\n",  p->love    * 100 / t);
        fprintf(fp, "Neutral  : %d%%\n",  p->neutral * 100 / t);
        fprintf(fp, "\n");
    }

    fprintf(fp, "=====================================================\n");
    fprintf(fp, "ALL MESSAGES:\n");
    fprintf(fp, "=====================================================\n");
    for (i = 0; i < total_messages; i++) {
        fprintf(fp, "[%s] %-16s | %-12s | %s\n",
                messages[i].date,
                messages[i].sender,
                mood_name(messages[i].mood),
                messages[i].message);
    }

    fclose(fp);
    printf("\n  Report saved to: mood_report.txt\n");
    printf("  Open it with Notepad to read your full report!\n");
    pause_screen();
}

/* ---------------------------------------------------
   MAIN MENU
   --------------------------------------------------- */
void print_menu() {
    clear_screen();
    printf("=====================================================\n");
    printf("    WHATSAPP CHAT SENTIMENT & MOOD ANALYZER\n");
    printf("       Decode the Mood Behind the Message\n");
    printf("=====================================================\n");
    printf("  [1] Load WhatsApp Chat from File (.txt)\n");
    printf("  [2] Type a Message Manually\n");
    printf("  [3] Run Demo Mode  (no file needed)\n");
    printf("  [4] Show Mood Report Per Person\n");
    printf("  [5] Show All Messages with Mood\n");
    printf("  [6] Show Chat Statistics\n");
    printf("  [7] Search Messages by Mood\n");
    printf("  [8] Save Report to File\n");
    printf("  [0] Exit\n");
    printf("=====================================================\n");
    if (data_loaded)
        printf("  Status: %d messages loaded | %d people\n",
               total_messages, total_people);
    else
        printf("  Status: No data loaded yet\n");
    printf("=====================================================\n");
    printf("  Enter your choice: ");
}

/* ---------------------------------------------------
   MAIN
   --------------------------------------------------- */
int main() {
    int choice;

    clear_screen();
    printf("  =====================================================\n");
    printf("    Welcome to WhatsApp Chat Mood Analyzer!\n");
    printf("    Decode the Mood Behind the Message\n");
    printf("  =====================================================\n");
    printf("    Press ENTER to start...");
    getchar();

    while (1) {
        print_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                load_chat_file();
                break;
            case 2:
                type_message_manually();
                break;
            case 3:
                load_demo();
                break;
            case 4:
                if (!data_loaded) {
                    printf("\n  Please load data first (options 1, 2, or 3).\n");
                    pause_screen();
                } else {
                    show_mood_report();
                }
                break;
            case 5:
                if (!data_loaded) {
                    printf("\n  Please load data first (options 1, 2, or 3).\n");
                    pause_screen();
                } else {
                    show_all_messages();
                }
                break;
            case 6:
                if (!data_loaded) {
                    printf("\n  Please load data first (options 1, 2, or 3).\n");
                    pause_screen();
                } else {
                    show_statistics();
                }
                break;
            case 7:
                if (!data_loaded) {
                    printf("\n  Please load data first (options 1, 2, or 3).\n");
                    pause_screen();
                } else {
                    search_by_mood();
                }
                break;
            case 8:
                if (!data_loaded) {
                    printf("\n  Please load data first (options 1, 2, or 3).\n");
                    pause_screen();
                } else {
                    save_report();
                }
                break;
            case 0:
                clear_screen();
                printf("  Thank you for using WhatsApp Mood Analyzer!\n");
                printf("  Goodbye!\n\n");
                return 0;
            default:
                printf("\n  Invalid! Enter a number between 0 and 8.\n");
                pause_screen();
        }
    }
    return 0;
}
