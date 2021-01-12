public class centralServer {
    public static int shared_sum = 0;
    public static int request = 0;
    public static int respond = 0;
    public static int finished = 0;
    public static int COUNT = 100;
    public static int MAX_THREADS = 4;
    
    
    static class Server extends Thread{
        public Server(){
        }

        public void run(){
            try {
                System.out.println("Server started");
                while (finished == 0){

                    while(request == 0 && finished == 0){
                        //
                        Thread.yield();
                    }
                    respond = request;
                    
                    while(respond != 0){
                        //
                        Thread.yield();
                    }

                    request = 0;
                }
                System.out.println("Server stopped");
                Thread.sleep(50);
            } catch(InterruptedException e){
                System.out.println("error found");
            }            
            System.out.println("Server thread exited");
        }
    }
    
    //static class Client implements Runnable
    static class Client extends Thread{
        private int thread_id;

        public Client(int _thread_id){
            thread_id = _thread_id;
        }

        public void run(){
            try {
                System.out.println("Thread " + thread_id + " started");
                /*while(respond != thread_id){
                    request = thread_id;
                    Thread.yield();
                }*/

                for (int i = 0; i < (COUNT/MAX_THREADS); i++){
                    //critical
                    
                    shared_sum = shared_sum + 1;
                    System.out.println("Thread " + thread_id + " is summing... Value so far is: " + shared_sum);

                    

                }
                respond = 0;
                Thread.sleep(50);
            }            
            catch (InterruptedException e){
                System.out.println("error found");
            }
            System.out.println("Client thread " + thread_id + " exited");
        }

    }

    public static void main(String args[]) throws InterruptedException{

         
        Thread[] client_thread_array = new Thread[MAX_THREADS];

        Thread server = new Server();

        long start_time = System.currentTimeMillis();

        server.start();        
        
        for (int i = 0; i < MAX_THREADS; i++){
            client_thread_array[i] = new Thread(new Client(i));
            client_thread_array[i].start();
        }
        
        try {
            for (int j = 0; j < MAX_THREADS; j++){
                client_thread_array[j].join();
            }
            finished = 1;
            server.join();
        } catch(InterruptedException e){
            System.out.println("Error ocurred");
        }
        
        long final_time = System.currentTimeMillis() - start_time;
		System.out.println("Processing time ->" + final_time + "ms");
        System.out.println("Final value: " + shared_sum);
        
    }
}
