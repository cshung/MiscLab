﻿namespace SuffixTreePrototype
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Text;

    // Still O(m^3) - building suffix links
    class SuffixTree4
    {
        private static bool assertOn = true;

        private SuffixTree4(string text)
        {
            // Avoid direct construction
            this.text = text;
        }

        public static SuffixTree4 Build(string text)
        {
            SuffixTree4 result = new SuffixTree4(text);
            result.BuildTree(text);
            return result;
        }

        private void BuildTree(string text)
        {
            // Step 1: Build initial tree
            this.rootNode = new SuffixTreeNode(0);
            string suffix = text[0] + "";
            this.fullStringLeaf = new SuffixTreeNode(1);
            this.rootNode.links.Add(suffix[0], new SuffixTreeLink { Start = 0, End = 1, Child = fullStringLeaf });

            // Step 2: Basic extension loop
            for (int endIndex = 2; endIndex <= text.Length; endIndex++)
            {
                for (int startIndex = 0; startIndex < endIndex; startIndex++)
                {
                    this.Extend(text, startIndex, endIndex);
                }
            }
        }

        // startIndex, endIndex are 0 based (] index to the string to be inserted to the current implicit suffix tree
        private void Extend(string text, int startIndex, int endIndex)
        {
            int insertingTextLength = endIndex - startIndex;
            int searchingTextLength = insertingTextLength - 1;
            int remainingSearchingTextLength = searchingTextLength;
            int textCursor = startIndex;
            SuffixTreeNode nodeCursor = this.rootNode;
            SuffixTreeLink followingLink = null;
            int linkCursor = -1;

            while (remainingSearchingTextLength > 0)
            {
                char currentCharacter = text[textCursor];
                if (followingLink == null)
                {
                    followingLink = nodeCursor.links[currentCharacter];
                    nodeCursor = null;
                    linkCursor = 0;
                }
                else
                {
                    if (linkCursor == followingLink.Length())
                    {
                        linkCursor = -1;
                        nodeCursor = followingLink.Child;
                        followingLink = null;
                    }
                    else
                    {
                        int move = Math.Min(followingLink.Length(), remainingSearchingTextLength);
                        if (assertOn)
                        {
                            string movedText = text.Substring(textCursor, move);
                            string movedLink = text.Substring(followingLink.Start, move);
                            Debug.Assert(string.Equals(movedText, movedLink));
                        }

                        textCursor += move;
                        remainingSearchingTextLength -= move;
                        linkCursor += move;
                    }
                }
            }

            SuffixTreeNode newlyCreatedInternalNode = null;

            char characterToExtend = text[endIndex - 1];
            if (followingLink != null)
            {
                if (linkCursor == followingLink.Length())
                {
                    if (followingLink.Child.links.Count == 0)
                    {
                        // Rule 1 - we have reached a leaf - extend the leaf, done
                        followingLink.End++;
                        followingLink.Child.StringDepth++;
                    }
                    else
                    {
                        if (followingLink.Child.links.ContainsKey(characterToExtend))
                        {
                            // Rule 3 - no op
                        }
                        else
                        {
                            // Rule 2
                            followingLink.Child.links.Add(characterToExtend, new SuffixTreeLink { Start = endIndex - 1, End = endIndex, Child = new SuffixTreeNode(insertingTextLength) });
                        }
                    }
                }
                else
                {
                    if (followingLink.EdgeLabel(text, linkCursor) == characterToExtend)
                    {
                        // Rule 3 - no op
                    }
                    else
                    {
                        // Rule 2 
                        int originalEnd = followingLink.End;
                        SuffixTreeNode originalChild = followingLink.Child;
                        newlyCreatedInternalNode = followingLink.Child = new SuffixTreeNode(searchingTextLength);
                        followingLink.End = followingLink.Start + linkCursor;
                        followingLink.Child.links.Add(text[followingLink.Start + linkCursor], new SuffixTreeLink { Start = followingLink.Start + linkCursor, End = originalEnd, Child = originalChild });
                        followingLink.Child.links.Add(characterToExtend, new SuffixTreeLink { Start = endIndex - 1, End = endIndex, Child = new SuffixTreeNode(insertingTextLength) });
                    }
                }
            }
            else
            {
                if (nodeCursor.links.ContainsKey(characterToExtend))
                {
                    // Rule 3 - no op
                }
                else
                {
                    // Rule 2 again
                    nodeCursor.links.Add(characterToExtend, new SuffixTreeLink { Start = endIndex - 1, End = endIndex, Child = new SuffixTreeNode(insertingTextLength) });
                }
            }

            if (this.lastInternalNode != null)
            {
                if (followingLink != null)
                {
                    Debug.Assert(linkCursor == followingLink.Length(), "B) We should always end up in another node if we applied rule 2");
                    this.lastInternalNode.SuffixLink = followingLink.Child;
                }
                else
                {
                    this.lastInternalNode.SuffixLink = nodeCursor;
                }

                this.lastInternalNode = null;
            }

            if (newlyCreatedInternalNode != null)
            {
                this.lastInternalNode = newlyCreatedInternalNode;
            }

        }

        private class SuffixTreeLink
        {
            // 0 based [) index
            public int Start { get; set; }
            public int End { get; set; }
            public char EdgeLabel(string text, int index)
            {
                return text[this.Start + index];
            }

            public string EdgeLabel(string text)
            {
                return text.Substring(this.Start, this.Length());
            }

            public int Length()
            {
                return this.End - this.Start;
            }

            public SuffixTreeNode Child { get; set; }
        }

        private class SuffixTreeNode
        {
            public SuffixTreeNode(int stringDepth)
            {
                this.StringDepth = stringDepth;
            }

            public Dictionary<char, SuffixTreeLink> links = new Dictionary<char, SuffixTreeLink>();

            public SuffixTreeNode SuffixLink { get; set; }

            public int StringDepth { get; set; }

            internal void BuildString(string text, StringBuilder sb, int indent)
            {
                sb.Append(new string('+', indent));
                sb.AppendFormat("({0})", this.StringDepth);
                sb.AppendLine();
                    
                foreach (var link in links)
                {
                    sb.Append(new string('-', indent));
                    sb.AppendLine(link.Value.EdgeLabel(text));
                    link.Value.Child.BuildString(text, sb, indent + 2);
                }
            }
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            rootNode.BuildString(this.text, sb, 0);
            return sb.ToString();
        }

        private SuffixTreeNode rootNode;
        private SuffixTreeNode fullStringLeaf;
        private SuffixTreeNode lastInternalNode;
        private string text;
    }

    class Program
    {
        static void Main(string[] args)
        {
            SuffixTree4 result = SuffixTree4.Build("xabxac");
            Console.WriteLine(result);
        }
    }
}
