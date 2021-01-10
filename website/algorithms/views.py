from django.shortcuts import render, get_object_or_404, redirect
from django.http import HttpResponse
from django.contrib.auth import authenticate, login, logout
from django.views.generic import View, ListView, DetailView
from django.views.generic.edit import CreateView
from django.core.files import File
from .models import Algorithm, Outcome
from .forms import UserForm, LoginUserForm, CompareAlgorithms
import mpld3
import matplotlib.pyplot as plt
import matplotlib.style as mst
import sys
sys.path.append('../FileManagement/')

from FileManagement import szkielet


class DetailView(DetailView):
    model = Algorithm
    template_name = 'algorithms/detail.html'

    def get_object(self):
        alg = super().get_object()

        if not alg.counted:
            dimensions = szkielet.generateResults("media/" + str(alg.alg_file))
            rownum = 1
            for dim in dimensions:
                with open('static/result_files/results' + str(dim) + '.txt', 'r') as myfile:
                    rownum = 1
                    for line in myfile:
                        out = Outcome()
                        out.algorithm = alg
                        out.dimension = dim
                        out.function = rownum
                        word_number = 1
                        for word in line.split():
                            if word_number == 1:
                                out.best = word
                            elif word_number == 2:
                                out.worst = word
                            elif word_number == 3:
                                out.median = word
                            elif word_number == 4:
                                out.mean_str = word
                                out.mean_float = float(word)
                            else:
                                out.std = word
                            
                            word_number = word_number + 1

                        rownum = rownum + 1
                        out.save()
                    
            alg.num_of_fun = rownum - 1
            alg.counted = True
            alg.save()
        return alg


class AlgorithmListView(ListView):
    model = Algorithm

    def get_queryset(self):
        return Algorithm.objects.all()


def home(request):
   return render(request, 'algorithms/home.html' )


class UserFormView(View):
    form_class = UserForm
    template_name = 'algorithms/registration.html'

    # wyswietla formularz
    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, { 'form' : form })
    
    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)

        if form.is_valid():
            user = form.save(commit=False)
            username = form.cleaned_data['username']
            password = form.cleaned_data['password']
            user.set_password(password)
            user.is_active=False
            user.save()

            if user is not None:
                return redirect('algorithms:wait')
            
        return render(request, self.template_name, { 'form' : form })


class LoginView(View):

    form_class = LoginUserForm
    template_name = 'algorithms/login.html'

    # wyswietla formularz
    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, { 'form' : form })
    
    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)

        if form.is_valid():
            username = request.POST['username']
            password = request.POST['password']

            user = authenticate(request, username = username, password = password)
            if user is not None:
                if user.is_active:
                    login(request, user)
                    return redirect('algorithms:home')
                else:       
                    return render(request, 'algorithms/wait-for-activate.html' )
            
        return render(request, self.template_name, { 'form' : form })

def wait(request):
    return render(request, 'algorithms/wait-for-activate.html' )


class AddAlgorithm(CreateView):
    model = Algorithm
    fields = ['name', 'author', 'alg_file']


def logoutView(request):
    logout(request)
    return render(request, 'algorithms/logout.html')


class CompAlgView(View):
    form_class = CompareAlgorithms
    template_name = 'algorithms/compare.html'

    # wyswietla formularz
    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, { 'form' : form })
    
    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)
        if form.is_valid():
            alg1 = Algorithm.objects.get(pk = request.POST['algorithm1'])
            alg2 = Algorithm.objects.get(pk = request.POST['algorithm2'])

            function = int(request.POST['function'])
            dimension = int(request.POST['dimension'])

            file_name1 = str(alg1.alg_file)
            file_name2 = str(alg2.alg_file)
            plt.clf()
            fig = plt.figure(dpi=175)
            for x in [1,2]:
                if x == 1:
                    mean_val = szkielet.calculateConvergenceGraph("media/" + file_name1, function, dimension)
                else:
                    mean_val = szkielet.calculateConvergenceGraph("media/" + file_name2, function, dimension)

                #x_axis = [0.01, 0.02, 0.03, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]
                x_axis = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]
                y_axis = mean_val


                #usuwanie najwiekszych wartosci dla czytelnosci
                while y_axis[0] > 150000:
                   x_axis.pop( max(x_axis) )
                   y_axis.__delitem__(0)

                
                if x == 1:
                    plt.scatter( x_axis, y_axis, c='darkred', marker='o' )
                    plt.plot( x_axis, y_axis, c='black', label="error value of   " + alg1.name )
                else:
                    plt.scatter( x_axis, y_axis, c='darkgreen', marker='o' )
                    plt.plot( x_axis, y_axis, c='darkviolet', label="error value of   " + alg2.name )

            mst.use('seaborn')
            plt.title( "Dimension: " + str(dimension) )
            plt.xlabel("FES")
            plt.ylabel("Mean Error Value")
            plt.ylim(bottom = -10)
            plt.grid(True)
            plt.legend()
            fig = plt.gcf()
            fig_html = mpld3.fig_to_html( fig )
        return render(request, self.template_name, { 'form' : form, 'fig_html' : fig_html })

