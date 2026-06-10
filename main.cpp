 // main.cpp - Productivity & Wellness Assistant Project
// Ye file tumhare poore Productivity & Wellness Assistant system ka source code hai.

// Zaroori C++ Standard Libraries ko include kar rahe hain.
#include <iostream>     // Input/Output operations ke liye, jaise console par messages print karna (std::cout).
#include <string>       // String (text) data type use karne ke liye, jaise window titles ko store karna.
#include <vector>       // Dynamic arrays (vectors) use karne ke liye, jaise syllabus ke tasks ki list.
#include <chrono>       // Time se related operations ke liye, jaise durations ko measure karna. (Though yahan Sleep() use ho raha hai, ye ek good practice hai).
#include <thread>       // Multi-threading operations ke liye, jaise program ko thodi der ke liye pause karna (std::this_thread::sleep_for).
#include <algorithm>    // Algorithms jaise sorting (std::sort) use karne ke liye, tasks ko priority ke hisaab se sort karne ke liye.
#include <fstream>      // File operations ke liye, jaise data ko CSV file mein save karna (ofstream).
#include <ctime>        // Current time aur date ko access karne ke liye, data logging mein timestamp add karne ke liye.
#include <iomanip>      // Output formatting ke liye, jaise numbers ko fixed decimal places tak print karna.

// Windows API specific headers. Ye functions Windows operating system se interact karne ke liye hain.
#include <windows.h>    // Windows API ki core functionalities, jaise Sleep, GetForegroundWindow, MessageBoxA, GetTickCount.
#include <winuser.h>    // Windows user interface functions ke liye, jaise GetLastInputInfo. (Ye aksar windows.h mein hi included hota hai, but explicit mention clarity ke liye achha hai).

// Enum (enumeration) class banaya hai, jo alag-alag activity categories ko define karta hai.
// Isse code zyada readable aur maintainable banta hai, numbers ki jagah meaningful names use kar sakte hain.
enum class ActivityCategory {
    Study,         // Agar user padhai kar raha hai (jaise VS Code, PDF reader).
    Entertainment, // Agar user entertainment kar raha hai (jaise Netflix, YouTube, Instagram).
    Other          // Agar activity na padhai hai na entertainment (jaise desktop, file explorer).
};

// --- Phase 1: Core C++ Logic (Basic Structure) ---
// --- Phase 2: Mathematical Model for Scheduling (Fatigue Calculation) ---

// FatigueModel class: Ye user ki thakaan (fatigue) ko estimate karta hai.
// Ye Phase 2 ke "Mathematical Model" ka practical implementation hai.
class FatigueModel {
public:
    // calculateFatigue function: User ki thakaan ki value calculate karta hai.
    // hoursWorked: User ne kitne ghante padhai ki hai.
    // entertainmentTime: User ne kitne ghante entertainment kiya hai.
    double calculateFatigue(double hoursWorked, double entertainmentTime) {
        // Ye ek simple linear approximation hai fatigue ko calculate karne ke liye.
        // Alpha (0.15) aur Beta (0.05) coefficients hain.
        // Phase 3 ke "AI/ML Integration" mein, ye coefficients (alpha aur beta)
        // ek real AI/ML model (jaise Linear Regression) dwara user ke habits ke hisaab se learn kiye jaate.
        // Abhi ke liye, ye fixed values hain demo purpose ke liye.
        // Padhai se thakaan badhti hai (hoursWorked * 0.15).
        // Controlled entertainment se thakaan thodi kam hoti hai (entertainmentTime * 0.05).
        return (hoursWorked * 0.15) - (entertainmentTime * 0.05);
    }
};

// Task struct: Har ek padhai ke task ko represent karta hai.
// Ye Phase 1 ke "Core Implementation" ka hissa hai.
struct Task {
    std::string name;             // Task ka naam, jaise "Electrical Machines".
    double durationHours;         // Task ko complete karne mein kitne ghante lagenge.
    bool isCompleted;             // Kya task complete ho gaya hai (true/false).
    int priority;                 // Task ki priority (lower number = higher priority, jaise 1 sabse high).
};

// StudyAssistant class: Ye poore productivity aur wellness system ka core engine hai.
// Ye Phase 1, 2, aur 3 ki functionalities ko integrate karta hai.
class StudyAssistant {
private:
    // Private members (variables) jo sirf StudyAssistant class ke andar access ho sakte hain.
    std::vector<Task> syllabus;       // Saare tasks ki list (syllabus).
    double dailyLimit;                // User ka daily total working/studying limit (ghanto mein).
    double entertainmentQuota;        // User ko kitna entertainment time allowed hai (ghanto mein).
    double timeSpentEntertainment;    // User ne kitna entertainment time bita diya hai.
    double timeSpentStudy;            // User ne kitna study time bita diya hai.
    FatigueModel brainModel;          // FatigueModel ka object, thakaan calculate karne ke liye.

    // --- Phase 3: Health and Stress Monitoring (Idle Time Detection) ---
    LASTINPUTINFO lastInputInfo;      // Windows structure jo last user input (keyboard/mouse) ki information rakhti hai.
    DWORD lastActivityTickCount;      // Last activity check ke time ka system tick count (GetTickCount() se milta hai).
    double idleThresholdMinutes;      // Kitne minutes tak idle rehne par warning deni hai.
    ActivityCategory lastActivityCategory; // User last time kis category (Study/Entertainment) mein active tha, idle time check ke liye.

    // --- Phase 3: AI/ML Integration (Data Collection & Analytical Layer) ---
    std::string logFileName;          // Activity data ko save karne ke liye CSV file ka naam.
    double efficiencyScore;           // User ki current efficiency score (Analytical Layer ka part).
                                      // Ye score user ki productivity ko reflect karta hai.

public:
    // Constructor: Jab StudyAssistant object banega, ye initial values set karega.
    // limit: Daily total limit, entertainment: Entertainment quota, idleThreshold: Idle time limit.
    // logFile: Data logging ke liye CSV file ka naam.
    StudyAssistant(double limit, double entertainment, double idleThreshold, std::string logFile = "activity_log.csv")
        : dailyLimit(limit), entertainmentQuota(entertainment), // Member initializer list se values set ki.
          timeSpentEntertainment(0), timeSpentStudy(0),        // Initial values 0 set ki.
          idleThresholdMinutes(idleThreshold),                  // Idle threshold set kiya.
          lastActivityCategory(ActivityCategory::Other),        // Shuruat mein last activity 'Other' set ki.
          logFileName(logFile),                                 // Log file ka naam set kiya.
          efficiencyScore(100.0)                                // Initial efficiency score 100% set kiya.
    {
        // LASTINPUTINFO structure ko initialize kiya.
        // Iske 'cbSize' member ko structure ke size se set karna zaroori hai Windows API ke liye.
        lastInputInfo.cbSize = sizeof(LASTINPUTINFO);
        // GetLastInputInfo function ko call karke current last input info li.
        GetLastInputInfo(&lastInputInfo);
        // Current system tick count ko store kiya.
        lastActivityTickCount = GetTickCount();

        // Log file ko setup kiya (agar file exist nahi karti to header likha).
        // std::ios::app mode mein file kholi taaki naya data existing data ke end mein add ho.
        std::ofstream logFileStream(logFileName, std::ios::app);
        // Agar file khuli hai aur uska size 0 hai (matlab file empty hai ya abhi bani hai),
        // to CSV header (column names) likho.
        if (logFileStream.is_open() && logFileStream.tellp() == 0) {
            logFileStream << "Timestamp,Activity,Window Title,Time Spent Study (h),Time Spent Entertainment (h),Fatigue,Efficiency Score\n";
        }
        logFileStream.close(); // File ko band kiya.
    }

    // addSyllabusTask function: Syllabus mein naya task add karta hai.
    void addSyllabusTask(std::string name, double hours, int prio = 3) {
        // syllabus vector mein naya Task object add kiya, initially uncompleted.
        syllabus.push_back({name, hours, false, prio});
    }

    // monitorActivity function: Real-time mein user ki current activity ko monitor karta hai.
    // Ye Phase 1 ke "Monitoring Layer" aur Phase 3 ke "Activity Classifier" ka core hai.
    void monitorActivity() {
        // GetForegroundWindow() function active (foreground) window ka handle (ID) deta hai.
        HWND foregroundWindow = GetForegroundWindow();
        char windowTitleCStr[256]; // Window title store karne ke liye character array.

        // GetWindowTextA() function active window ka title fetch karta hai.
        // 'A' suffix ANSI character set ke liye hai.
        GetWindowTextA(foregroundWindow, windowTitleCStr, sizeof(windowTitleCStr));
        std::string currentWindowTitle = windowTitleCStr; // char array ko std::string mein convert kiya.

        // Idle time detection ke liye last activity time ko update kiya.
        GetLastInputInfo(&lastInputInfo); // Current last input info fetch ki.
        lastActivityTickCount = GetTickCount(); // Current system tick count store kiya.

        // ActivityCategory enum ka variable banaya, default 'Other' set kiya.
        ActivityCategory currentCategory = ActivityCategory::Other;

        // Window title mein specific keywords search karke activity categorize ki.
        // Ye "Activity Classifier (ML Model)" ka simplified version hai.
        // Real ML model yahan window title, process name, aur user behavior se predict karta.
        if (currentWindowTitle.find("Netflix") != std::string::npos || // string::npos matlab 'not found'.
            currentWindowTitle.find("YouTube") != std::string::npos ||
            currentWindowTitle.find("Spotify") != std::string::npos ||
            currentWindowTitle.find("Game") != std::string::npos ||
            currentWindowTitle.find("Discord") != std::string::npos ||
            currentWindowTitle.find("Facebook") != std::string::npos ||
            currentWindowTitle.find("Instagram") != std::string::npos ||
            currentWindowTitle.find("WhatsApp") != std::string::npos ||
            // Browsers mein specific entertainment URLs ko detect kiya.
            (currentWindowTitle.find("Google Chrome") != std::string::npos && (currentWindowTitle.find("youtube.com") != std::string::npos || currentWindowTitle.find("netflix.com") != std::string::npos || currentWindowTitle.find("facebook.com") != std::string::npos || currentWindowTitle.find("instagram.com") != std::string::npos)) ||
            (currentWindowTitle.find("Microsoft Edge") != std::string::npos && (currentWindowTitle.find("youtube.com") != std::string::npos || currentWindowTitle.find("netflix.com") != std::string::npos || currentWindowTitle.find("facebook.com") != std::string::npos || currentWindowTitle.find("instagram.com") != std::string::npos))
            ) {
            currentCategory = ActivityCategory::Entertainment; // Category 'Entertainment' set ki.
        }
        else if (currentWindowTitle.find("Visual Studio Code") != std::string::npos ||
                   currentWindowTitle.find("Word") != std::string::npos ||
                   currentWindowTitle.find("PDF") != std::string::npos ||
                   currentWindowTitle.find("PowerPoint") != std::string::npos ||
                   currentWindowTitle.find("Excel") != std::string::npos ||
                   currentWindowTitle.find("Jupyter") != std::string::npos ||
                   currentWindowTitle.find("Terminal") != std::string::npos || // Command Prompt/PowerShell/WSL
                   currentWindowTitle.find("Git Bash") != std::string::npos ||
                   currentWindowTitle.find("Zoom Meeting") != std::string::npos || // Online classes/meetings
                   currentWindowTitle.find("Microsoft Teams") != std::string::npos ||
                   currentWindowTitle.find("Outlook") != std::string::npos || // Email for work/study
                   (currentWindowTitle.find("Google Chrome") != std::string::npos && // Browsers mein specific study URLs ko detect kiya.
                    (currentWindowTitle.find("google.com/search") != std::string::npos || // Google search for study
                     currentWindowTitle.find("stackoverflow.com") != std::string::npos ||
                     currentWindowTitle.find("docs.microsoft.com") != std::string::npos ||
                     currentWindowTitle.find("geeksforgeeks.org") != std::string::npos ||
                     currentWindowTitle.find("leetcode.com") != std::string::npos ||
                     currentWindowTitle.find("w3schools.com") != std::string::npos ||
                     currentWindowTitle.find("wikipedia.org") != std::string::npos || // Research
                     currentWindowTitle.find("coursera.org") != std::string::npos ||
                     currentWindowTitle.find("udemy.com") != std::string::npos ||
                     currentWindowTitle.find("edx.org") != std::string::npos ||
                     currentWindowTitle.find("scholar.google.com") != std::string::npos))) {
            currentCategory = ActivityCategory::Study; // Category 'Study' set ki.
        }

        // Categorized activity ke hisaab se time spent aur efficiency score update kiya.
        // Har 5 second ke loop mein, 0.1 hours (6 minutes) add/subtract ho rahe hain.
        if (currentCategory == ActivityCategory::Entertainment) {
            timeSpentEntertainment += 0.1; // Entertainment time badhaya.
            checkEntertainmentLimit();     // Entertainment limit check ki.
            lastActivityCategory = ActivityCategory::Entertainment; // Last activity category update ki.
            efficiencyScore -= 0.5; // Entertainment se efficiency score thoda kam kiya.
        } else if (currentCategory == ActivityCategory::Study) {
            timeSpentStudy += 0.1;         // Study time badhaya.
            lastActivityCategory = ActivityCategory::Study;         // Last activity category update ki.
            efficiencyScore += 0.2; // Study se efficiency score thoda badhaya.
        } else {
            // Agar activity na study hai na entertainment, to time spent update nahi karte.
            // Lekin lastActivityCategory ko change nahi karte taaki idle time detection sahi se kaam kare.
            efficiencyScore -= 0.1; // Other activities se efficiency score thoda kam kiya.
        }
        
        // Efficiency score ko 0-100 ki range mein rakha, taaki value valid rahe.
        if (efficiencyScore < 0) efficiencyScore = 0;
        if (efficiencyScore > 100) efficiencyScore = 100;

        // Har monitoring cycle mein idle time bhi check kiya (Phase 3).
        checkIdleTime();

        // Current activity data ko log file mein save kiya (Phase 3: Data Collection).
        logActivityData(currentCategory, currentWindowTitle);
    }

    // checkEntertainmentLimit function: Check karta hai ki entertainment limit exceed hui hai ya nahi.
    // Ye Phase 2 ke "Rescheduling Algorithm" trigger ka ek part hai.
    void checkEntertainmentLimit() {
        if (timeSpentEntertainment > entertainmentQuota) {
            // MessageBoxA() function Windows pop-up message box dikhata hai.
            // NULL: Owner window handle (NULL matlab desktop).
            // "Message": Pop-up ka text.
            // "Title": Pop-up ka title bar text.
            // MB_OK | MB_ICONWARNING: OK button aur warning icon dikhao.
            MessageBoxA(NULL, "You have exceeded your entertainment limit! Please return to study.", "Productivity Assistant Warning", MB_OK | MB_ICONWARNING);
            redesignSchedule(); // Limit exceed hone par schedule redesign kiya.
            efficiencyScore -= 10.0; // Limit exceed hone par efficiency score significantly kam kiya.
        }
    }

    // redesignSchedule function: Time deviation hone par schedule ko adjust karta hai.
    // Ye Phase 2 ke "Scheduling Layer" ka implementation hai.
    // Real Dynamic Programming ya priority queues yahan aur complex logic use karte.
    void redesignSchedule() {
        std::cout << "[AI] Redesigning schedule due to time deviation..." << std::endl;
        
        // Pending tasks ki list banayi.
        std::vector<Task*> pendingTasks;
        for (auto& task : syllabus) { // syllabus ke har task par loop chalao.
            if (!task.isCompleted) { // Agar task pura nahi hua hai.
                pendingTasks.push_back(&task); // Pending tasks ki list mein add kiya.
            }
        }

        // Pending tasks ko priority ke hisaab se sort kiya.
        // Lambda expression use ki hai sorting ke liye: lower priority number = higher importance.
        std::sort(pendingTasks.begin(), pendingTasks.end(), [](Task* a, Task* b) {
            return a->priority < b->priority; // 'a' ki priority 'b' se kam hai to 'a' pehle aayega.
        });

        std::cout << "-> Recommended Tasks (by Priority):" << std::endl;
        if (pendingTasks.empty()) {
            std::cout << "   No pending tasks. Good job!" << std::endl;
        } else {
            for (const auto& taskPtr : pendingTasks) {
                std::cout << "   - " << taskPtr->name
                          << " (Priority: " << taskPtr->priority
                          << ", Est. " << taskPtr->durationHours << "h remaining)" << std::endl;
            }
        }
    }

    // healthCheck function: Fatigue level check karta hai aur break suggest karta hai.
    // Ye Phase 1 ke "Analytical Layer" aur Phase 3 ke "Health Monitoring" ka part hai.
    void healthCheck() {
        // FatigueModel se current fatigue calculate ki.
        double fatigue = brainModel.calculateFatigue(timeSpentStudy, timeSpentEntertainment);
        if (fatigue > 0.7) { // Agar fatigue level 0.7 se zyada hai (ye value adjust kar sakte ho).
            // Pop-up message box dikhaya break lene ke liye.
            MessageBoxA(NULL, "High Fatigue detected. Please take a 15-min break.", "Productivity Assistant Health Alert", MB_OK | MB_ICONINFORMATION);
            efficiencyScore -= 5.0; // High fatigue se efficiency score kam kiya.
        }
    }

    // checkIdleTime function: User ke idle time ko check karta hai.
    // Ye Phase 3 ke "Health and Stress Monitoring" ka part hai.
    void checkIdleTime() {
        LASTINPUTINFO lii = { sizeof(LASTINPUTINFO) }; // LASTINPUTINFO structure banaya aur size set kiya.
        GetLastInputInfo(&lii); // Last user input ki information li.
        
        // GetTickCount() current system uptime milliseconds mein deta hai.
        // lii.dwTime last input ka tick count hai.
        // Difference se idle time milliseconds mein mila.
        DWORD idleTimeMs = GetTickCount() - lii.dwTime;
        // Milliseconds ko minutes mein convert kiya.
        double idleTimeMinutes = idleTimeMs / (1000.0 * 60.0);

        // Agar idle time set threshold se zyada hai.
        if (idleTimeMinutes > idleThresholdMinutes) {
            // Warning tabhi deni hai jab user pehle 'Study' activity mein tha.
            // Isse faltu warnings nahi aayengi agar user entertainment ke baad idle hai.
            if (lastActivityCategory == ActivityCategory::Study) {
                MessageBoxA(NULL, "You've been idle for a while during study. Are you experiencing cognitive fatigue? Take a break!", "Productivity Assistant Idle Alert", MB_OK | MB_ICONINFORMATION);
                // Warning dene ke baad lastActivityCategory ko 'Other' set kiya
                // taaki baar-baar pop-up na aaye jab tak user phir se active na ho.
                lastActivityCategory = ActivityCategory::Other; 
                efficiencyScore -= 3.0; // Idle hone se efficiency score kam kiya.
            }
        }
        // Phase 3: Keystroke Dynamics (Future Enhancement)
        // Yahan par keystroke dynamics ko monitor kiya ja sakta hai.
        // Jaise: Rapid, erratic typing high stress indicate kar sakta hai.
        // Iske liye low-level keyboard hooks aur advanced analysis ki zaroorat padegi,
        // jo is single-file demo ke scope se bahar hai.
    }
// logActivityData function: Current activity details ko CSV file mein log karta hai.
    // Ye Phase 3 ke "Data Collection" part ko simulate karta hai AI/ML training ke liye.
    void logActivityData(ActivityCategory category, const std::string& windowTitle) {
        std::ofstream logFileStream(logFileName, std::ios::app); // Append mode mein file kholi.
        if (logFileStream.is_open()) {
            // Current time aur date ko format kiya.
            std::time_t now = std::time(nullptr);
            char timestamp[100];
            // strftime function time ko specific format mein convert karta hai.
            std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

            // Log entry ko CSV format mein likha.
            logFileStream << timestamp << ","; // Timestamp
            if (category == ActivityCategory::Study) {
                logFileStream << "Study,";
            } else if (category == ActivityCategory::Entertainment) {
                logFileStream << "Entertainment,";
            } else {
                logFileStream << "Other,";
            }
            logFileStream << "\"" << windowTitle << "\","; // Window title mein commas ho sakte hain, isliye quotes mein rakha.
            logFileStream << std::fixed << std::setprecision(2) << timeSpentStudy << ","; // Study time (2 decimal places)
            logFileStream << std::fixed << std::setprecision(2) << timeSpentEntertainment << ","; // Entertainment time (2 decimal places)
            logFileStream << std::fixed << std::setprecision(2) << brainModel.calculateFatigue(timeSpentStudy, timeSpentEntertainment) << ","; // Fatigue (2 decimal places)
            logFileStream << std::fixed << std::setprecision(2) << efficiencyScore << "\n"; // Efficiency Score (2 decimal places)
            logFileStream.close(); // File ko band kiya.
        } else {
            // Agar log file open nahi ho payi to console par error message print kiya.
            std::cerr << "Error: Could not open log file " << logFileName << std::endl;
        }
    }
};

// main function: Program ka entry point. Execution yahan se shuru hoti hai.
int main() {
    // Console par starting message print kiya.
    std::cout << "Productivity Assistant Starting (Real-time Monitoring Mode)..." << std::endl;
    std::cout << "Minimize this window to run in background. Close it to exit." << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
    // User ko initial message padhne ke liye 2 seconds ka pause diya.
    Sleep(2000); // Windows API function, 2000 milliseconds = 2 seconds.

    // StudyAssistant object banaya.
    // 8.0: Daily total work/study limit 8 ghante.
    // 1.5: Entertainment quota 1.5 ghante.
    // 10.0: Idle time threshold 10 minutes.
    // "activity_log.csv": Data logging ke liye file ka naam.
    StudyAssistant myAI(8.0, 1.5, 10.0, "activity_log.csv"); 
    
    // Syllabus mein tasks add kiye (name, duration, priority).
    // Priority 1 sabse zyada important hai, 3 sabse kam.
    myAI.addSyllabusTask("C++ Basics", 2.0, 1); // High priority task
    myAI.addSyllabusTask("C++ Data Structures", 3.0, 2); // Medium priority task
    myAI.addSyllabusTask("Project Documentation", 1.5, 3); // Lower priority task
    myAI.addSyllabusTask("CS Fundamentals", 1.0, 1); // Another high priority task
    myAI.addSyllabusTask("Prepare Presentation", 2.5, 1); // High priority task
    myAI.addSyllabusTask("Review Code", 3.0, 2); // Medium priority task

    // Continuous Monitoring Loop: Ye loop program ko indefinitely chalata rahega.
    // Jab tak program forcefully band nahi kiya jata (jaise console window close karke ya Ctrl+C press karke), ye chalta rahega.
    while (true) {
        myAI.monitorActivity(); // Current active window ko monitor karo.
        myAI.healthCheck();     // Fatigue level check karo aur break suggest karo.
        
        // Har 5 seconds (5000 milliseconds) mein activity check karo.
        // Tum is time ko adjust kar sakti ho (jaise 1000ms for 1 second, 10000ms for 10 seconds).
        Sleep(5000); // Windows API function to pause execution.
    }

    // Ye 'return 0;' statement tabhi execute hogi jab infinite loop kisi tarah se break ho.
    // Normally, background assistant programs mein ye line nahi reach hoti, jab tak program forcefully terminate na ho.
    return 0;
}