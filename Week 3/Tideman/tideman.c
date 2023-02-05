#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void merge_sort(int start, int end, pair to_sort[]);
void sort_pairs(void);
bool no_cycle(int win, int lose);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Iterate each candidates's name
    for (int i = 0; i < candidate_count; i++)
    {
        // Update ranks and return true if name match
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Iterate row
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // Iterate column
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Update preferences based on ranks
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Initialize pair_count
    pair_count = 0;

    // Iterate row
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // Iterate column
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Merge sort
void merge_sort(int start, int end, pair to_sort[])
{
    // Base case
    if (end <= start)
    {
        return;
    }

    int mid = (start + end) / 2;

    // Sort left half(start to mid)
    merge_sort(start, mid, to_sort);

    // Sort righ half(mid + 1 to end)
    merge_sort(mid + 1, end, to_sort);

    // Create tmp pairs to store
    pair tmp[pair_count];

    // Merge
    for (int i = start, j = start, k = mid + 1; i <= end; i++)
    {
        // Left half nothing left
        if (j > mid)
        {
            tmp[i] = to_sort[k];
            k++;
        }
        // Righ half nothing left
        else if (k > end)
        {
            tmp[i] = to_sort[j];
            j++;
        }
        // Compare and copy to tmp, then move to next number
        else if (preferences[to_sort[j].winner][to_sort[j].loser] >
                 preferences[to_sort[k].winner][to_sort[k].loser])
        {
            tmp[i] = to_sort[j];
            j++;
        }
        else
        {
            tmp[i] = to_sort[k];
            k++;
        }
    }

    // Cover tmp to to_sort
    for (int i = start; i <= end; i++)
    {
        to_sort[i] = tmp[i];
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    merge_sort(0, pair_count - 1, pairs);
    return;
}

// Check cycle
// Reference: https://gist.github.com/nicknapoli82/6c5a1706489e70342e9a0a635ae738c9
bool no_cycle(int win, int lose)
{
    // Base case
    if (win == lose)
    {
        return false;
    }

    // Re-examine all the pairs
    for (int i = 0; i < candidate_count; i++)
    {
        // Newly found loser win against other candidate
        if (locked[lose][i])
        {
            if (no_cycle(win, i) == false)
            {
                return false;
            }
        }
    }
    return true;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // Lock if no cycle
        if (no_cycle(winner, loser))
        {
            locked[winner][loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // Iterate row
    for (int i = 0; i < candidate_count; i++)
    {
        // Initialize counter
        int pointed_counter = candidate_count;

        // Iterate column
        for (int j = 0; j < candidate_count; j++)
        {
            // Decrease counter if no pointed at
            if (locked[j][i] == false)
            {
                pointed_counter--;
            }
            // Print final winner if no one pointed at
            if (pointed_counter == 0)
            {
                printf("%s\n", candidates[i]);
            }
        }
    }
    return;
}