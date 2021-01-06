public class centralServer {
    public static int shared_sum = 0;
    public static int request = 0;
    public static int respond = 0;
    public static int finished = 0;
    public static int COUNT = 20;
    
    //static class Server implements Runnable
    static class Server extends Thread{
        public Server(){
        }

        public void run(){
            try {
                System.out.println("Server started");
                while (finished == 0){

                    /*while(request != 0){
                        //
                        System.out.println("Server request  " + request);
                        Thread.sleep(50);
                        Thread.yield();
                    }
                    respond = request;*/
                    
                    Thread.yield();
                    if (request != 0){
                        
                        respond = request;
                    }
                    
                    Thread.yield();
                    if (respond == 0 ){
                        request = 0;
                    }

                    /*while(respond == 0){
                        //
                        System.out.println("Server respond  " + respond);
                        Thread.sleep(50);
                        Thread.yield();
                    }

                    request = 0;*/
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
                for (int i = 0; i < COUNT; i++){
                    while(respond != thread_id){
                        request = thread_id;
                        Thread.yield();
                    }

                    //critical
                    shared_sum = shared_sum + 2;

                    respond = 0;

                }
                Thread.sleep(50);
            }            
            catch (InterruptedException e){
                System.out.println("error found");
            }
            System.out.println("Client thread " + thread_id + " exited");
        }

    }

    public static void main(String args[]) throws InterruptedException{

        int MAX_THREADS = 4; 
        Thread[] client_thread_array = new Thread[MAX_THREADS];

        Thread server = new Server();
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
        

        System.out.println("Final value: " + shared_sum);
    }
}
