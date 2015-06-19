namespace SuffixTreePrototype
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Text;

    // Still O(m^3) - building suffix links
    class SuffixTree
    {
        private static bool assertOn = true;

        private SuffixTree(string text)
        {
            // Avoid direct construction
            this.text = text;
        }

        public static SuffixTree Build(string text)
        {
            SuffixTree result = new SuffixTree(text);
            result.BuildTree(text);
            return result;
        }

        private void BuildTree(string text)
        {
            // Step 1: Build initial tree
            this.rootNode = new SuffixTreeNode(0, 1);
            this.fullStringLeaf = new SuffixTreeNode(1, 1);
            SuffixTreeLink nodeToFullStringLeafLink = new SuffixTreeLink { Parent = rootNode, Start = 0, End = 1, Child = fullStringLeaf };
            this.rootNode.links.Add(text[0], nodeToFullStringLeafLink);
            fullStringLeaf.ParentLink = nodeToFullStringLeafLink;

            // Step 2: Basic extension loop
            for (int endIndex = 2; endIndex <= text.Length; endIndex++)
            {
                this.lastExtensionLeafNode = null;
                for (int startIndex = 0; startIndex < endIndex; startIndex++)
                {
                    if (this.Extend(text, startIndex, endIndex))
                    {
                        // Short circuit to the next phase if rule 3 ever applied
                        break;
                    }
                }
            }
        }

        // startIndex, endIndex are 0 based [) index to the string to be inserted to the current implicit suffix tree
        private bool Extend(string text, int startIndex, int endIndex)
        {
            int insertingTextLength = endIndex - startIndex;
            int searchingTextLength = insertingTextLength - 1;

            //
            // Step 1: Search for the end of the searchingText
            //         The search result could be 
            //         nodeCursor == rootNode, in this case the searchingText is the empty string
            //         followingLink != null and 0 < linkCursor <= followingLink.Length(), that represents the exclusive end index of a pointer on the link
            //         note that if linkCursor == followingLink.Length(), it means we are at the followingLink.child node
            //
            // We decided to use followingLink != null && linkCursor == followingLink.Length() to represent the cursor is at the next node is because we need 
            // to manipulate the link at times (e.g. extending it in leaf case)
            // 
            SuffixTreeNode nodeCursor = this.rootNode;
            SuffixTreeLink followingLink = null;
            int linkCursor = -1;

            if (startIndex == 0)
            {
                // For the first extension - it is always extending the full string leaf node
                followingLink = this.fullStringLeaf.ParentLink;
                linkCursor = followingLink.Length();
            }
            else
            {
                int remainingSearchingTextLength = searchingTextLength;
                int textCursor = startIndex;
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
            }
            
            //
            // Extension algorithm - rule 1, 2, 3
            //
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
                            // Rule 3 - no op - and be done with the phase
                            return true;
                        }
                        else
                        {
                            // Rule 2 - but not creating internal nodes
                            SuffixTreeNode newLeaf = new SuffixTreeNode(insertingTextLength, 1);
                            SuffixTreeLink newExtendingLink = new SuffixTreeLink { Parent = followingLink.Child, Start = endIndex - 1, End = endIndex, Child = newLeaf };
                            newLeaf.ParentLink = newExtendingLink;
                            followingLink.Child.links.Add(characterToExtend, newExtendingLink);
                            this.lastExtensionLeafNode = newLeaf;
                        }
                    }
                }
                else
                {
                    if (followingLink.EdgeLabel(text, linkCursor) == characterToExtend)
                    {
                        // Rule 3 - no op - and be done with the phase
                        return true;
                    }
                    else
                    {
                        // Rule 2 - creating internal node
                        int originalEnd = followingLink.End;
                        SuffixTreeNode originalChild = followingLink.Child;
                        newlyCreatedInternalNode = followingLink.Child = new SuffixTreeNode(searchingTextLength, 1);
                        newlyCreatedInternalNode.ParentLink = followingLink;
                        followingLink.End = followingLink.Start + linkCursor;
                        SuffixTreeLink brokenLink2 = new SuffixTreeLink { Parent = followingLink.Child, Start = followingLink.Start + linkCursor, End = originalEnd, Child = originalChild };
                        originalChild.ParentLink = brokenLink2;
                        followingLink.Child.links.Add(text[followingLink.Start + linkCursor], brokenLink2);

                        SuffixTreeNode newLeaf = new SuffixTreeNode(insertingTextLength, 1);
                        SuffixTreeLink newExtendingLink = new SuffixTreeLink { Parent = followingLink.Child, Start = endIndex - 1, End = endIndex, Child = newLeaf };
                        newLeaf.ParentLink = newExtendingLink;
                        followingLink.Child.links.Add(characterToExtend, newExtendingLink);
                    }
                }
            }
            else
            {
                if (nodeCursor.links.ContainsKey(characterToExtend))
                {
                    // Rule 3 - no op - and be done with the phase
                    return true;
                }
                else
                {
                    // Rule 2 - but not creating internal nodes
                    SuffixTreeNode newLeaf = new SuffixTreeNode(insertingTextLength, 1);
                    SuffixTreeLink newExtendingLink = new SuffixTreeLink { Parent = nodeCursor, Start = endIndex - 1, End = endIndex, Child = newLeaf };
                    newLeaf.ParentLink = newExtendingLink;
                    nodeCursor.links.Add(characterToExtend, newExtendingLink);
                }
            }

            if (this.lastInternalNode != null)
            {
                if (followingLink != null)
                {
                    Debug.Assert(linkCursor == followingLink.Length(), "We should always end up in another node if we applied rule 2");
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

            return false;
        }

        private class SuffixTreeLink
        {
            // 0 based [) index
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

            public SuffixTreeNode Parent { get; set; }
            public int Start { get; set; }
            public int End { get; set; }
            public SuffixTreeNode Child { get; set; }
        }

        private class SuffixTreeNode
        {
            public SuffixTreeNode(int stringDepth, int fuckedAgain)
            {
                this.StringDepth = stringDepth;
            }

            public Dictionary<char, SuffixTreeLink> links = new Dictionary<char, SuffixTreeLink>();

            public SuffixTreeLink ParentLink { get; set; }

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
        private SuffixTreeNode lastExtensionLeafNode;
        private string text;
    }

    class Program
    {
        static void Main(string[] args)
        {
            SuffixTree result = SuffixTree.Build("xabxac");
            Console.WriteLine(result);
        }
    }
}
