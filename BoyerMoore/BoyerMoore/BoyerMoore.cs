using System;
using System.Collections.Generic;

namespace BoyerMoore
{
    // Prevent Visual Studio Live Unit Testing crashing the dotnet process on Assert
    public static class Debug
    {
        public static void Assert(bool condition)
        {
            if (!condition)
            {
                throw new Exception();
            }
        }
    }

    public static class BoyerMoore
    {
        private const bool logging = false;

        public static IEnumerable<int> IndexesOf(string text, string pattern)
        {
            if (pattern.Equals(""))
            {
                yield break;
            }

            //
            // For an initial read of the algorithm, it is advisable to read this later, just assume you have a magic
            // shift table computed
            //
            int[] goodSuffixShifts;
            int[] goodSuffixSkips;
            BuildGoodSuffixShifts(pattern, out goodSuffixShifts, out goodSuffixSkips);

            Dictionary<char, int[]> badCharacterShifts = BuildBadCharacterShifts(pattern);

            int i = 0;
            int goodSuffixCheck = -1;
            int goodSuffixSkip = 0;
            while (i + pattern.Length <= text.Length)
            {
                // Consider the pattern is placed so that its first character it under the ith character of the text
                // We started the matching from the right of the pattern
                int patternIndex = pattern.Length - 1;
                int textIndex = i + patternIndex;

                while (patternIndex >= 0 && text[textIndex] == pattern[patternIndex])
                {
                    textIndex--;
                    patternIndex--;

                    // Here is an optimization for the good suffix case
                    // If the shift is caused by a good suffix, we first count down the 
                    // number of characters shifted, as we knew nothing about the text for 
                    // those shifts
                    if (goodSuffixCheck > 0)
                    {
                        goodSuffixCheck--;

                        // Once we reach the point where we hit the known match
                        if (goodSuffixCheck == 0)
                        {
                            // Then we just skip all the known matches 
                            goodSuffixCheck = -1;
                            textIndex -= goodSuffixSkip;
                            patternIndex -= goodSuffixSkip;
                        }
                    }
                }

                // If the loop ended with patternIndex == -1
                if (patternIndex == -1)
                {
                    // The whole pattern matched
                    yield return i;
                }

                // Figure out how many characters we can shift the pattern
                int matched = pattern.Length - patternIndex - 1;
                Debug.Assert(matched <= pattern.Length);

                if (logging)
                {
                    Console.WriteLine($"Aligning at position {i} we matched {matched} characters");
                    Console.WriteLine(text);
                    for (int j = 0; j < i; j++)
                    {
                        Console.Write(' ');
                    }
                    Console.WriteLine(pattern);
                }

                int badCharacterShift = 0;
                if (patternIndex != -1)
                {
                    // In case we have a mismatching character
                    char badCharacter = text[textIndex];
                    if (badCharacterShifts.ContainsKey(badCharacter))
                    {
                        // In case the bad character does exist in the pattern
                        // We find the right most occurence of it and align them
                        badCharacterShift = badCharacterShifts[badCharacter][patternIndex];
                    }
                    else
                    {
                        // Otherwise, the bad character is not seen, we can shift the pattern 
                        // so that it is after the bad character
                        badCharacterShift = patternIndex + 1;
                    }
                }

                // Because we already knew a suffix matches, we can use the goodSuffixShifts table
                // to determine how much shift is possible, the table guarantees the shift never
                // miss a match. See BuildGoodSuffixShifts() to understand how that table is constructed.
                int goodSuffixShift = goodSuffixShifts[matched];

                if (badCharacterShift > goodSuffixShift)
                {
                    // In case we have a bad character shift, we know nothing about what will match
                    // after the shift (except, of course, the only character we are aligning to)
                    // so we switch the goodSuffixCheck off
                    goodSuffixCheck = -1;
                    goodSuffixSkip = 0;
                    i += badCharacterShift;
                }
                else
                {
                    // In case we have a good suffix shift, we know the shifted characters are unknown
                    // so they must be checked
                    goodSuffixCheck = goodSuffixShift;
                    goodSuffixSkip = goodSuffixSkips[matched];
                    i += goodSuffixShift;
                }
            }
        }

        private static Dictionary<char, int[]> BuildBadCharacterShifts(string pattern)
        {
            Dictionary<char, int[]> badCharacterShifts = new Dictionary<char, int[]>();
            for (int i = 0; i < pattern.Length; i++)
            {
                char c = pattern[i];
                int[] shift;
                if (!badCharacterShifts.TryGetValue(c, out shift))
                {
                    // A new array is by default zero filled
                    shift = new int[pattern.Length];
                    badCharacterShifts.Add(c, shift);
                }

                // Therefore, we have a 1 in the array at the corresponding index if the 
                // character appears, and 0 otherwise
                shift[i] = 1;
            }

            foreach (int[] shift in badCharacterShifts.Values)
            {
                // A placeholder value to indicate the array is never shifted
                int lastShift = -1;
                for (int i = 0; i < shift.Length; i++)
                {
                    if (shift[i] == 0)
                    {
                        // At this point we are at a mismatch
                        if (lastShift == -1)
                        {
                            // There is no occurrence of the character before this position
                            // Therefore we can safely shift the pattern past the character
                            shift[i] = i + 1;
                        }
                        else
                        {
                            // Last time we shifted lastShift characters and then it aligns
                            // Therefore we can shift one more character now to align
                            shift[i] = ++lastShift;
                        }
                    }
                    else
                    {
                        // Here we have as hit, the driver should not access this value
                        shift[i] = -1;
                        // And we need to shift no character to achieve alignment now
                        lastShift = 0;
                    }
                }
            }

            return badCharacterShifts;
        }

        private static void BuildGoodSuffixShifts(string pattern, out int[] shifts, out int[] skips)
        {
            //
            // In this method, we are going to produce a table so that we know how many characters we can shift
            // the pattern after we know a certain number of characters matches in the suffix given only the pattern
            // 
            // We will exploit the self-similarity of the pattern. In particular, we wanted to know when a substring
            // also matches a suffix. 
            //
            // We define a substring pattern[i, j) is a suffix if pattern[i, j) == pattern[i - j + length, length)
            // 
            // At a first glance, it is unclear where does the term i - j + length comes from? It comes from the fact we 
            // want j to become length, so one must subtract j and plus length, that's why we have the term. We will use
            // this algebra repeatedly.
            // 
            // Now we define a suffix pattern[i, j) to be a maximal suffix if i == 0 or pattern[i - 1, j) is not a suffix.
            // i.e. one cannot extend this anymore to the left. Maximal suffix is useful because it is a compact encoding
            // for all suffixes.
            // 
            // Without further ado, the first part of the algorithm find all the maximal suffixes.
            // If pattern[i, j) is a maximal suffix, then maximalSuffixLengths[j - 1] = j - i = length of the maximal suffix. 
            // Otherwise the array entry is 0.
            //
            int length = pattern.Length;
            int[] maximalSuffixLengths = FindMaximalSuffixes(pattern);
            //
            // Within the set of maximal suffixes, there is a special set of maximal suffixes that starts at position 0
            // They are defined to be borders. For reasons that we will make a clear later, we wanted to know the longest 
            // length border smaller or equals to a given length.
            //
            // longestBorder[i] = j means the length of a longest border contained by a suffix of length i + 1 is j.
            //
            int[] longestBorder = FindLongestBorder(maximalSuffixLengths);
            //
            // With the information prepared above, we are ready to build the shifts table. Broadly, there are 3 cases:
            // 
            // Case A: The alignment results in no match
            // Case B: The alignment results in a partial match
            // Case C: The alignment results in a complete match
            //
            // Case A is particularly easy to deal with. We know nothing about what the last character is. We can only make
            // the most conservative choice, shifting by 1.
            //
            // Case B is tricky, considering the following general case:
            //
            // text    : .......X*****......
            // pattern :      ..Y*****
            //
            // Now we could have three types of shifts:
            //
            // Case B1: The pattern still cover the matched portion completely.
            // text    : .......X*****......
            // pattern :       ..Y*****
            //
            // Case B2: The pattern cover the matched portion only partially
            // text    : .......X*****......
            // pattern :          ..Y*****
            //
            // Case B3: The pattern does not cover the matched portion
            // text    : .......X*****......
            // pattern :              ..Y*****
            //
            // To make sure we will not miss a match, we need to as conservative as possible, so we consider smallest shift 
            // cases first
            //
            // For case B1 to make sense, I claim that the pattern must have shifted so that the matched portion aligns with
            // a maximal suffix. In order for the match to not fail, the matched portion must be a suffix. We knew the match
            // failed, so we know X is not equal to Y. If the suffix is not a maximal suffix, we will compare X with Y and 
            // fail again, so the suffix must be maximal.
            //
            // For case B2 to make sense, I claim that the pattern must have shifted so that a suffix matches a prefix. In this
            // case, we should pick the longest border that is smaller or equal to the length of the matched portion. The 
            // equal part is bogus, because it would have been case B1.
            // 
            // Case B3 is the ideal, we would love to shift that much, it applies only if case B1 and B2 doesn't.
            //
            // Case C is similar to case B, case B1 does not apply to case C as it is impossible to shift a completely matched
            // pattern and still completely matched, but B2 and B3 applies just fine for C. We will call that C2 and C3.
            //
            // The number of matched characters ranged from [0, length], therefore the shift table has length + 1 entries
            //
            shifts = new int[length + 1];
            skips = new int[length + 1];
            //
            // Handling case A
            //
            shifts[0] = 1;
            skips[0] = 0;
            //
            // Handling case B1, scanning from the right finding the rightmost maximal suffix of length i.
            //
            for (int i = length - 2; i >= 0; i--)
            {
                int maximalSuffixLength = maximalSuffixLengths[i];
                // If we found a maximal suffix
                if (maximalSuffixLength != 0)
                {
                    // And it is the rightmost one with length == maximalSuffixLength
                    if (shifts[maximalSuffixLength] == 0)
                    {
                        // We want to shift so that the ith character is shifted to the last character
                        // i.e. i + shift = length - 1
                        shifts[maximalSuffixLength] = length - 1 - i;
                        skips[maximalSuffixLength] = maximalSuffixLength;
                    }
                }
            }

            // Handling case B2, scanning the longest border to determine matches
            for (int i = 1; i < length; i++)
            {
                // If case B1 does not apply
                if (shifts[i] == 0)
                {
                    //
                    // If case B2 applies, remember longestBorder[i] = j means the the longest border with length
                    // less than or equals to i + 1 has length j
                    //
                    if (longestBorder[i - 1] != 0)
                    {
                        // We want to shift so that the last character of the longest border
                        // text    : .........AXX.
                        // pattern :      XX..AXX
                        // shifted :           XX..AXX

                        // TODO: I wonder if it makes sense to confirm the shift is actually 
                        // making the overlap shorter than the currently matched piece
                        shifts[i] = length - longestBorder[i - 1];
                        skips[i] = longestBorder[i - 1];
                    }
                }
            }

            // TODO: The length > 2 check is suspicious
            if (length > 2 && longestBorder[length - 2] != 0)
            {
                // If case C2 applies
                shifts[length] = longestBorder[length - 2] - 1;
                skips[length] = longestBorder[length - 2];
            }

            // Handling case B3 and C3
            for (int i = 0; i <= length; i++)
            {
                if (shifts[i] == 0)
                {
                    shifts[i] = length;
                    skips[i] = 0;
                }
            }

            if (logging)
            {
                Console.WriteLine("Maximal Suffix: ");
                Console.WriteLine(pattern);
                for (int i = 0; i < length; i++)
                {
                    Console.Write(maximalSuffixLengths[i]);
                }
                Console.WriteLine();

                Console.WriteLine("Longest border:");
                Console.WriteLine(pattern);
                for (int i = 0; i < length; i++)
                {
                    Console.Write(longestBorder[i]);
                }
                Console.WriteLine();

                Console.WriteLine("Shift:");
                Console.WriteLine(pattern);
                for (int i = length; i >= 0; i--)
                {
                    Console.Write(shifts[i]);
                }
                Console.WriteLine();

                Console.WriteLine("Skip:");
                Console.WriteLine(pattern);
                for (int i = length; i >= 0; i--)
                {
                    Console.Write(skips[i]);
                }
                Console.WriteLine();
            }
        }

        private static int[] FindMaximalSuffixes(string pattern)
        {
            // 
            // In this method, we are going to find all the maximal suffixes. 
            // Conceptually, we are looping through all possible exclusive ending index and 
            // search towards the beginning of the string while maintaining it matches with the suffix
            //
            int length = pattern.Length;
            int[] maximalSuffixLengths = new int[length];
            //
            // [maximalStart, maximalEnd) is the last found left most maximal suffix, it is used to speed up the 
            // maximal suffix search. Since it is impossible for maximalStart to be length, it is used
            // to mark the fact there is no last found left most maximal suffix yet.
            //
            int maximalStart = length;
            int maximalEnd = length;
            for (int currentEnd = length - 1; currentEnd > 0; currentEnd--)
            {
                //
                // In each iteration of this loop, we find currentStart such that [currentStart, currentEnd) is a 
                // a maximal suffix, at any time, we make sure [currentStart, currentEnd) = [suffixStart, length)
                // 
                // Both substring are initialized to be empty
                //
                int currentStart = currentEnd;
                int suffixStart = length;

                //
                // Since currentEnd is at most length - 1, this condition will not match until we find a maximal suffix
                // and that the last character of the 'to be' maximal suffix lies within it.
                //
                // From a correctness standpoint, this if-statement can be excluded, this is purely an optimization.
                // This optimization can improve the worst case complexity for finding all maximal suffix from
                // O(|pattern|^2) to O(|pattern|)
                //
                if (maximalStart < currentEnd && currentEnd <= maximalEnd)
                {
                    //
                    // We have already done some comparison before, can we skip some now? 
                    // Let's take stock on what we have:
                    //
                    // First of all, we knew we had a previously found left most maximal suffix
                    //
                    Debug.Assert(IsMaximalSuffix(pattern, maximalStart, maximalEnd));

                    //
                    // we know [maximalStart, maximalEnd) = [maximalStart - maximalEnd + length, length)
                    // Therefore [maximalStart, currentEnd) = [maximalStart - maximalEnd + length, currentEnd - maximalEnd + length)
                    //                                      = [knownStart, knownEnd)
                    //
                    int knownStart = maximalStart - maximalEnd + length;
                    int knownEnd = currentEnd - maximalEnd + length;

                    //
                    // What do we know about the latter? First of all, we know it is on the right to the currentEnd
                    //
                    Debug.Assert(currentEnd < knownEnd && knownEnd <= length);

                    //
                    // Since we build up the maximalSuffixLengths from right to left, we know the length of the maximal
                    // suffix that ends at knownEnd.
                    //
                    int knownSuffixLength = maximalSuffixLengths[knownEnd - 1];

                    //
                    // With the length and the end index, we can compute knownSuffixStart so that [knownSuffixStart, knownEnd) 
                    // is a maximal suffix
                    //
                    int knownSuffixStart = knownEnd - knownSuffixLength;
                    Debug.Assert(knownSuffixStart >= 0);
                    Debug.Assert(IsMaximalSuffix(pattern, knownSuffixStart, knownEnd));

                    //
                    // We knew [maximalStart, currentEnd) = [knownStart, knownEnd) and [knownSuffixStart, knownEnd) is 
                    // a maximal suffix.
                    // 
                    // Therefore we know [?, currentend) = [max(knownStart, knownSuffixStart), knownEnd) is a suffix
                    // 
                    // It remains to find the ?, which is easy because we can simply match length
                    //
                    int maxStart = Math.Max(knownSuffixStart, knownStart);
                    int maxEnd = knownEnd;
                    int maxLength = maxEnd - maxStart;
                    currentStart -= maxLength;
                    suffixStart -= maxLength;

                    Debug.Assert(IsSuffix(pattern, maxStart, knownEnd));
                    Debug.Assert(pattern.SubstringStartEnd(currentStart, currentEnd).Equals(pattern.SubstringStartEnd(maxStart, maxEnd)));
                    Debug.Assert(IsSuffix(pattern, currentStart, currentEnd));
                }

                // Now, we extend the suffix until we cannot so that the suffix will be maximal

                // Note that in this loop, we are either exploring characters already discovered in a known maximal suffix
                // in which the loop should terminate right away because the maximal suffix terminated inside, or we are 
                // discovering new characters. Therefore the total time spent on this loop should be proportional to the length of pattern
                do
                {
                    currentStart--;
                    suffixStart--;
                }
                while (currentStart >= 0 && pattern[currentStart] == pattern[suffixStart]);

                // We moved one extra character, adjust back
                currentStart++;

                // Now we have found the maximal suffix
                Debug.Assert(IsMaximalSuffix(pattern, currentStart, currentEnd));

                // Book keeping for the left most maximal suffix
                if (currentStart < maximalStart)
                {
                    maximalStart = currentStart;
                    maximalEnd = currentEnd;
                }

                // And save the lengths
                maximalSuffixLengths[currentEnd - 1] = currentEnd - currentStart;
            }

            // Note that the loop above does not attempt to compute the maximal suffix of
            // the whole string. This particular case is trivial.
            maximalSuffixLengths[length - 1] = length;

            // Make sure we have got it right in debug mode
            for (int i = 0; i < length; i++)
            {
                Debug.Assert(IsMaximalSuffix(pattern, i + 1 - maximalSuffixLengths[i], i + 1));
            }

            return maximalSuffixLengths;
        }

        private static int[] FindLongestBorder(int[] maximalSuffixLengths)
        {
            int length = maximalSuffixLengths.Length;
            int[] longestBorder = new int[length];
            int borderLength = 0;
            for (int i = 0; i < length; i++)
            {
                if (maximalSuffixLengths[i] == i + 1)
                {
                    borderLength = i + 1;
                }
                longestBorder[i] = borderLength;
            }
            return longestBorder;
        }

        private static string SubstringStartEnd(this string s, int start, int end)
        {
            if (start == s.Length)
            {
                Debug.Assert(end == s.Length);
                return "";
            }
            else
            {
                Debug.Assert(start >= 0);
                Debug.Assert(end >= 0);
                Debug.Assert(end <= s.Length);
                Debug.Assert(end >= start);
                return s.Substring(start, end - start);
            }
        }

        private static bool IsSuffix(string s, int start, int end)
        {
            return s.SubstringStartEnd(start, end) == s.SubstringStartEnd(start + s.Length - end, s.Length);
        }

        private static bool IsMaximalSuffix(string s, int start, int end)
        {
            if (IsSuffix(s, start, end))
            {
                if (start > 0)
                {
                    return !IsSuffix(s, start - 1, end);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }
    }
}
