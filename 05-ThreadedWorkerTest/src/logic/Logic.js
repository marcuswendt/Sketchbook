

var Logic = function()
{
	var frame = 0;
    var x = 0;
    var y = 0;
    
	this.update = function()
	{
//		print("Logic#update.... "+ frame);

		var bignum = 1000 * 1000 * 1;
		var sum = 0;
		for(var i=0; i<bignum; i++) {
			sum += Math.sqrt(sum+i);
		}
    
        x = Math.sin(frame*0.1) * 250 + 500;
        y = Math.cos(frame*0.1) * 250 + 350;
        
        frame++;
	}
    
    this.x = function() { return x; }
    this.y = function() { return y; }
    
    this.frame = function()
    {
        return frame;
    }
}