void listAll(const MenuItem* m, string path) // two-parameter overload
{
    //You will write this code.
    string nextPath = path;
    
    if (m->name() != ""){
        nextPath += m->name();
        std::cout << nextPath << std::endl;
        nextPath += "/";
    }
    
    const vector<MenuItem*>* next = m->menuItems();
    
    if (next == NULL)
        return;
    
    for (int a = 0; a < (*next).size(); a++)
        listAll((*next)[a], nextPath);
}
