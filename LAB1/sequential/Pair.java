package sequential;

public class Pair<left, right> {
    private left l;
    private right r;

    public Pair(left l, right r){
        this.l = l;
        this.r = r;
    }

    public left getLeft(){ 
        return l; 
    }

    public right getRight(){ 
        return r; 
    }

    public void setLeft(left l){ 
        this.l = l; 
    }
    
    public void setRight(right r){ 
        this.r = r; 
    }
}